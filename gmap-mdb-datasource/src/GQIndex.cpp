#include "GQindex.h"
#include "GGeometry.h"

#include "GPoint.h"
#include "GPolygon.h"
#include "GMultiGeometry.h"

#include <Gdiplus.h>
#include <assert.h>
#include "GMapDrawing.h"

namespace gmap{

	GQIndexRasterizer::GQIndexRasterizer(void)
	{
		CachedBitmap = new Gdiplus::Bitmap(16,16);
		CachedGraphics = Graphics::FromImage(CachedBitmap);
		CachedPointCount = 2048;
		CachedPoints = new Point[CachedPointCount];

		CachedPen = new Pen(Color::Black,1);
		CachedBrush = new SolidBrush(Color::Black);
	}

	GQIndexRasterizer::~GQIndexRasterizer(void)
	{
		delete CachedBitmap;
		delete CachedGraphics;
		delete[] CachedPoints;
		delete CachedPen;
		delete CachedBrush;
	}

	//!初始化
	void GQIndexRasterizer::Initialize(const GRectangle2dd& bbox)
	{
		f64 D = (bbox.GetHeight()> bbox.GetWidth()?bbox.GetHeight():bbox.GetWidth());
		AdjustedBBox = GRectangle2dd(bbox.MinEdge,bbox.MinEdge+GVector2dd(D,D));
		BBoxDimension = GVector2dd(D,D);
		BBoxMinEdge = bbox.MinEdge;
	}

	//!结束
	void GQIndexRasterizer::Uninitialize()
	{


	}

	//!得到修正过的包围盒
	const GRectangle2dd GQIndexRasterizer::GetAdjustedBoundingBox()
	{
		return AdjustedBBox;
	}

	//!得到不同层次单元尺寸
	f64	  GQIndexRasterizer::GetLevelPixelSize(u32 level)
	{
		return BBoxDimension.X/(0x8<<level);
	}
	
	//!得到大小
	u32  GQIndexRasterizer::GetLevelDimension(u32 level)
	{
		return 0x8<<level;
	}

	inline Point GetPixelPosition(f64 x,f64 y,f64 x0,f64 y0,f64 d)
	{
		int xx=(x-x0)/d;
		int yy=(y-y0)/d;
		return Point(xx,yy);
	}
	inline Point GetPixelPosition2(f64 x,f64 y,f64 x0,f64 y0,f64 d)
	{
		int xx=(x-x0)/d*2;
		int yy=(y-y0)/d*2;
		return Point(xx,yy);
	}
	inline void GQIndexRasterizer::RasterizeGeometry(Gdiplus::Graphics* graphics,const GVector2dd& minEdge,GLineString* lineString,f64 d)
	{
		GRectangle2dd box = lineString->GetBoundingBox();
		//!TODO:在静态内存中分配，可以降低C++内存分配的负担	

		u32 count = lineString->GetCoordinateCount();

		if(CachedPointCount<count)
		{			
			delete[] CachedPoints;
			CachedPoints =new Point[count];
			CachedPointCount=count;
		}


		for(int i=0;i<count;i++)
		{
			const GVector2dd& pt = lineString->GetCoordinateAt(i);
			CachedPoints[i] = GetPixelPosition2(pt.X,pt.Y,minEdge.X,minEdge.Y,d);
		}

		CachedGraphics->DrawLines(CachedPen,CachedPoints,count);

	}

	inline void GQIndexRasterizer::RasterizeGeometry(Gdiplus::Graphics* graphics,const GVector2dd& minEdge,GPolygon* polygon,f64 d)
	{
		GRectangle2dd box = polygon->GetBoundingBox();
		//!在静态内存中分配，可以降低C++内存分配的负担	

		GLinearRing* linearRing = polygon->GetExternalRing();
		u32 count = linearRing->GetCoordinateCount();

		if(CachedPointCount<count)
		{			
			delete[] CachedPoints;
			CachedPoints =new Point[count];
			CachedPointCount=count;
		}

		GraphicsPath path;

		for(int i=0;i<count;i++)
		{
			const GVector2dd& pt = linearRing->GetCoordinateAt(i);
			CachedPoints[i] = GetPixelPosition2(pt.X,pt.Y,minEdge.X,minEdge.Y,d);
		}

		path.AddPolygon(CachedPoints,count);

		for(int k=0;k<polygon->GetInternalRingCount();k++)
		{
			linearRing = polygon->GetInternalRingAt(k);
			count = linearRing->GetCoordinateCount();

			if(CachedPointCount<count)
			{			
				delete[] CachedPoints;
				CachedPoints =new Point[count];
				CachedPointCount=count;
			}

			for(int i=0;i<count;i++)
			{
				const GVector2dd& pt = linearRing->GetCoordinateAt(i);
				CachedPoints[i] = GetPixelPosition2(pt.X,pt.Y,minEdge.X,minEdge.Y,d);
			}

			path.AddPolygon(CachedPoints,count);
		}

		CachedGraphics->FillPath(CachedBrush,&path);
		CachedGraphics->DrawPath(CachedPen,&path);
	}

	//!光栅化
	GQINDEX  GQIndexRasterizer::RasterizeGeometry(GGeometry* pGeometry)
	{
		GRectangle2dd box = pGeometry->GetBoundingBox();
		f64 d = box.GetHeight()>box.GetWidth()?box.GetHeight():box.GetWidth();

		u32 level = BBoxDimension.X /d;
		level = log((double)level)/log((double)2);

		f64 pixelSize = GetLevelPixelSize(level);
		u32 dimension = GetLevelDimension(level);
		Point point = GetPixelPosition(box.MinEdge.X,box.MinEdge.Y,BBoxMinEdge.X,BBoxMinEdge.Y,pixelSize);
		GVector2dd minEdge(BBoxMinEdge.X+point.X*pixelSize,BBoxMinEdge.Y+point.Y*pixelSize);
		
		if((box.MaxEdge.X-minEdge.X)>dimension*pixelSize
			||(box.MaxEdge.Y-minEdge.Y)>dimension*pixelSize)
		{
			level--;
			pixelSize = GetLevelPixelSize(level);
			dimension = GetLevelDimension(level);
			point = GetPixelPosition(box.MinEdge.X,box.MinEdge.Y,BBoxMinEdge.X,BBoxMinEdge.Y,pixelSize);
		    minEdge =GVector2dd(BBoxMinEdge.X+point.X*pixelSize,BBoxMinEdge.Y+point.Y*pixelSize);
		}


		GQINDEX qindex;
		qindex.IMAGE=0;
		qindex.LEVEL = level;
		qindex.X=point.X;
		qindex.Y=point.Y;
		CachedGraphics->Clear(Color::White);

		if(pGeometry->GetGeometryType()==GGeometry::GT_LINE_STRING)
		{
			RasterizeGeometry(CachedGraphics,minEdge,(GLineString*)pGeometry,pixelSize);
		}
		else if(pGeometry->GetGeometryType()==GGeometry::GT_POLYGON)
		{
			RasterizeGeometry(CachedGraphics,minEdge,(GPolygon*)pGeometry,pixelSize);
		}
		else if(pGeometry->GetGeometryType()&GGeometry::GT_COLLECTION)
		{
			GMultiGeometry* pMultiGeometry = (GMultiGeometry*)pGeometry;
			for(int i=0;i<pMultiGeometry->GetGeometryCount();i++)
			{
				GGeometry* pGeometry2 = pMultiGeometry->GetGeometryAt(i);				
				if(pGeometry2->GetGeometryType()==GGeometry::GT_LINE_STRING)
				{
					RasterizeGeometry(CachedGraphics,minEdge,(GLineString*)pGeometry2,pixelSize);
				}
				else if(pGeometry2->GetGeometryType()==GGeometry::GT_POLYGON)
				{
					RasterizeGeometry(CachedGraphics,minEdge,(GPolygon*)pGeometry2,pixelSize);
				}
			}
		}

		for(int y=0;y<8;y++)
			for(int x=0;x<8;x++)
			{
				Color clr;
				CachedBitmap->GetPixel(x*2,y*2,&clr);
				if(clr.GetR()!=255){
					SetIndexPixelFlag(&qindex,x,y,true);
				}
				CachedBitmap->GetPixel(x*2+1,y*2,&clr);
				if(clr.GetR()!=255){
					SetIndexPixelFlag(&qindex,x,y,true);
				}
				CachedBitmap->GetPixel(x*2,y*2+1,&clr);
				if(clr.GetR()!=255){
					SetIndexPixelFlag(&qindex,x,y,true);
				}
				CachedBitmap->GetPixel(x*2+1,y*2+1,&clr);
				if(clr.GetR()!=255){
					SetIndexPixelFlag(&qindex,x,y,true);
				}				
			}
//CachedBitmap->Save(L"1.bmp",NULL);
			return qindex;
	}

	//!static
	bool GQIndexRasterizer::GetIndexPixelFlag(GQINDEX* pQIndex,u8 x,u8 y)
	{
		unsigned __int64 u = 0x1<<x;
		u<<=(y*8);
		return pQIndex->IMAGE&u;
	}

	//!static
	void GQIndexRasterizer::SetIndexPixelFlag(GQINDEX* pQIndex,u8 x,u8 y,bool flag)
	{		
		unsigned __int64 u = 0x1<<x;
		u<<=(y*8);
		if(!flag)
		{
			u=~u;			
			pQIndex->IMAGE=pQIndex->IMAGE&u;
		}
		else
		{
			pQIndex->IMAGE=pQIndex->IMAGE|u;
		}
	}

//------------------------------------------------------
//				
//					QCODE Op
//
//------------------------------------------------------

		//!盒子是否相交
		bool QCodeOp::IntersectBox( QCODE q1, QCODE q2)
		{
			u32  ca= 0x8<<(31-q1.LEVEL); 
			u32  cb= 0x8<<(31-q2.LEVEL);
			
			u32 ax0 = q1.X*ca;
			u32 ay0 = q1.Y*ca;
			u32 ax1 = ax0+8*ca;
			u32 ay1 = ay0+8*ca;

			u32 bx0 = q2.X*cb;
			u32 by0 = q2.Y*cb;
			u32 bx1 = bx0+8*cb;
			u32 by1 = by0+8*cb;

			return (bx0<ax1 && by0<ay1 && bx1>ax0 && by1<ay0);
		}
		
		QCODE QCodeOp::Align(QCODE q1,QCODE q2)
		{
			assert(q1.LEVEL == q2.LEVEL); //ASSERT
			int offsetx = q1.X-q2.X;
			int offsety = q1.Y-q2.Y;

			if(offsetx>0)
			{				
				q1.scan0<<=offsetx;q1.scan1<<=offsetx;
				q1.scan2<<=offsetx;q1.scan3<<=offsetx;
				q1.scan4<<=offsetx;q1.scan5<<=offsetx;
				q1.scan6<<=offsetx;q1.scan7<<=offsetx;
				q1.X-=offsetx;
			}
			else if(offsetx<0)
			{
				offsetx =-offsetx;
				q1.scan0>>=offsetx;q1.scan1>>=offsetx;
				q1.scan2>>=offsetx;q1.scan3>>=offsetx;
				q1.scan4>>=offsetx;q1.scan5>>=offsetx;
				q1.scan6>>=offsetx;q1.scan7>>=offsetx;
				q1.X+=offsetx;
			}

			if(offsety>=0)
			{
				q1.IMAGE<<=(offsety*8);
				q1.Y-=offsety;
			}
			else 
			{	
				offsety=-offsety;
				q1.IMAGE>>=(offsety*8);
				q1.Y+=offsety;
			}

			return q1;
		}
		//!Bitmap是否相交
		bool QCodeOp::IntersectBitmap( QCODE q1, QCODE q2)
		{
					assert(q1.LEVEL == q2.LEVEL); //ASSERT
			int offsetx = q1.X-q2.X;
			int offsety = q1.Y-q2.Y;

			if(offsetx>0)
			{				
				q1.scan0<<=offsetx;q1.scan1<<=offsetx;
				q1.scan2<<=offsetx;q1.scan3<<=offsetx;
				q1.scan4<<=offsetx;q1.scan5<<=offsetx;
				q1.scan6<<=offsetx;q1.scan7<<=offsetx;
				q1.X-=offsetx;
			}
			else if(offsetx<0)
			{
				offsetx =-offsetx;
				q1.scan0>>=offsetx;q1.scan1>>=offsetx;
				q1.scan2>>=offsetx;q1.scan3>>=offsetx;
				q1.scan4>>=offsetx;q1.scan5>>=offsetx;
				q1.scan6>>=offsetx;q1.scan7>>=offsetx;
				q1.X+=offsetx;
			}

			if(offsety>=0)
			{
				q1.IMAGE<<=(offsety*8);
				q1.Y-=offsety;
			}
			else 
			{	
				offsety=-offsety;
				q1.IMAGE>>=(offsety*8);
				q1.Y+=offsety;
			}


			return (q1.IMAGE & q2.IMAGE) !=0;
			
		}

		//!是否相交
		bool QCodeOp::Intersect(QCODE q1,QCODE q2)
		{
			if(q1.LEVEL>q2.LEVEL)
			{
				int level = q1.LEVEL-q2.LEVEL;
				QCODE qt=QCodeOp::DownL(q1,level);
				return QCodeOp::Intersect(qt,q2);
			}
			else if(q1.LEVEL<q2.LEVEL)
			{
				int level = q2.LEVEL-q1.LEVEL;
				QCODE qt=QCodeOp::DownL(q2,level);
				return QCodeOp::Intersect(q1,qt);
			}
			return IntersectBitmap(q1,q2);
		}
		u8 QCodeOp::LEVEL_1_0[256];
		u8 QCodeOp::LEVEL_1_1[256];

		u8 QCodeOp::LEVEL_2_0[256];
		u8 QCodeOp::LEVEL_2_1[256];
		u8 QCodeOp::LEVEL_2_2[256];
		u8 QCodeOp::LEVEL_2_3[256];
		
		u8 QCodeOp::LEVEL_3_0[256];
		u8 QCodeOp::LEVEL_3_1[256];
		u8 QCodeOp::LEVEL_3_2[256];
		u8 QCodeOp::LEVEL_3_3[256];
		u8 QCodeOp::LEVEL_3_4[256];
		u8 QCodeOp::LEVEL_3_5[256];
		u8 QCodeOp::LEVEL_3_6[256];
		u8 QCodeOp::LEVEL_3_7[256];
		
		QCODE QCodeOp::DownL(const QCODE& q,int level)
		{
			if(level==1)return DownL1(q);
			else if(level==2)return DownL2(q);
			else return DownL3(q,level);
		}

		//!降级1
		QCODE QCodeOp::DownL1(const QCODE& q)
		{
			int x = q.X/2;
			int y = q.Y/2;
			int offsetX = q.X%2;
			int offsetY = q.Y%2;
			u8 scan0,scan1,scan2,scan3,scan4,scan5,scan6,scan7;

			QCODE qt;
			qt.X = x;
			qt.Y = y;
			qt.LEVEL = q.LEVEL-1;
			if(offsetX==0)
			{
				scan0 =  QCodeOp::LEVEL_1_0[q.scan0];
				scan1 =  QCodeOp::LEVEL_1_0[q.scan1];
				scan2 =  QCodeOp::LEVEL_1_0[q.scan2];
				scan3 =  QCodeOp::LEVEL_1_0[q.scan3];
				scan4 =  QCodeOp::LEVEL_1_0[q.scan4];
				scan5 =  QCodeOp::LEVEL_1_0[q.scan5];
				scan6 =  QCodeOp::LEVEL_1_0[q.scan6];
				scan7 =  QCodeOp::LEVEL_1_0[q.scan7];
			}
			else
			{				
				scan0 =  QCodeOp::LEVEL_1_1[q.scan0];
				scan1 =  QCodeOp::LEVEL_1_1[q.scan1];
				scan2 =  QCodeOp::LEVEL_1_1[q.scan2];
				scan3 =  QCodeOp::LEVEL_1_1[q.scan3];
				scan4 =  QCodeOp::LEVEL_1_1[q.scan4];
				scan5 =  QCodeOp::LEVEL_1_1[q.scan5];
				scan6 =  QCodeOp::LEVEL_1_1[q.scan6];
				scan7 =  QCodeOp::LEVEL_1_1[q.scan7];
			}

			if(offsetY==0)
			{
				qt.scan0=scan0|scan1;
				qt.scan1=scan2|scan3;
				qt.scan2=scan4|scan5;
				qt.scan3=scan6|scan7;
				qt.scan4=0;
				qt.scan5=0;
				qt.scan6=0;
				qt.scan7=0;

			}
			else 
			{
				qt.scan0=scan0;
				qt.scan1=scan1|scan2;
				qt.scan2=scan3|scan4;
				qt.scan3=scan5|scan6;
				qt.scan4=scan7;
				qt.scan5=0;
				qt.scan6=0;
				qt.scan7=0;
			}
			return qt;
		}

		//!降级2
		QCODE QCodeOp::DownL2(const QCODE& q)
		{
			int x = q.X/4;
			int y = q.Y/4;
			int offsetX = q.X%4;
			int offsetY = q.Y%4;
			
			u8 scan0,scan1,scan2,scan3,scan4,scan5,scan6,scan7;

			QCODE qt;
			qt.X = x;
			qt.Y = y;
			qt.LEVEL = q.LEVEL-2;
			u8* convtable = NULL;
			if(offsetX==0)convtable =QCodeOp::LEVEL_2_0;
			else if(offsetX==1)convtable =QCodeOp::LEVEL_2_1;
			else if(offsetX==2)convtable =QCodeOp::LEVEL_2_2;
			else	convtable =QCodeOp::LEVEL_2_3;

			scan0 =  convtable[q.scan0];
			scan1 =  convtable[q.scan1];
			scan2 =  convtable[q.scan2];
			scan3 =  convtable[q.scan3];
			scan4 =  convtable[q.scan4];
			scan5 =  convtable[q.scan5];
			scan6 =  convtable[q.scan6];
			scan7 =  convtable[q.scan7];
			qt.IMAGE=0;
			if(offsetY==0)
			{
				qt.scan0=scan0|scan1|scan2|scan3;
				qt.scan1=scan4|scan5|scan6|scan7;
			}
			else if(offsetY==1)
			{
				qt.scan0=scan0|scan1|scan2;
				qt.scan1=scan3|scan4|scan5|scan6;
				qt.scan2=scan7;			
			}
			else if(offsetY==2)
			{
				qt.scan0=scan0|scan1;
				qt.scan1=scan2|scan3|scan4|scan5;
				qt.scan2=scan6|scan7;		
			}
			else
			{
				qt.scan0=scan0;
				qt.scan1=scan1|scan2|scan3|scan4;
				qt.scan2=scan5|scan6|scan7;				
			}
			return qt;
		}

		//!降级3
		QCODE QCodeOp::DownL3(const QCODE& q,int level)
		{
			int div = 0x1<<level;
			int x = q.X/div;
			int y = q.Y/div;
			int offsetX = q.X%div;
			int offsetY = q.Y%div;

			offsetX = offsetX+8-div;
			offsetY = offsetY+8-div;
			offsetX = offsetX>0?offsetX:0;
			offsetY = offsetY>0?offsetY:0;
			u8* convtable;
			switch(offsetX)
			{
				case 0:convtable = QCodeOp::LEVEL_3_0;break;
				case 1:convtable = QCodeOp::LEVEL_3_1;break;
				case 2:convtable = QCodeOp::LEVEL_3_2;break;
				case 3:convtable = QCodeOp::LEVEL_3_3;break;
				case 4:convtable = QCodeOp::LEVEL_3_4;break;
				case 5:convtable = QCodeOp::LEVEL_3_5;break;
				case 6:convtable = QCodeOp::LEVEL_3_6;break;
				default:convtable =QCodeOp::LEVEL_3_7;break;
			}
			
			u8 scan0,scan1,scan2,scan3,scan4,scan5,scan6,scan7;
			QCODE qt;
			qt.X = x;
			qt.Y = y;
			qt.IMAGE=0;
			qt.LEVEL=q.LEVEL-level;
			scan0=convtable[q.scan0];
			scan1=convtable[q.scan1];
			scan2=convtable[q.scan2];
			scan3=convtable[q.scan3];
			scan4=convtable[q.scan4];
			scan5=convtable[q.scan5];
			scan6=convtable[q.scan6];
			scan7=convtable[q.scan7];
			
			switch(offsetY)
			{
			case 0:{qt.scan0=scan0|scan1|scan2|scan3|scan4|scan5|scan6|scan7;};break;
			case 1:{
					qt.scan0=scan0|scan1|scan2|scan3|scan4|scan5|scan6;
					qt.scan1=scan7;
				   };break;
			case 2:{
					qt.scan0=scan0|scan1|scan2|scan3|scan4|scan5;
					qt.scan1=scan6|scan7;
				   };break;
			case 3:{					
					qt.scan0=scan0|scan1|scan2|scan3|scan4;
					qt.scan1=scan5|scan6|scan7;
				   };break;
			case 4:{					
					qt.scan0=scan0|scan1|scan2|scan3;
					qt.scan1=scan4|scan5|scan6|scan7;
				   };break;
			case 5:{					
					qt.scan0=scan0|scan1|scan2;
					qt.scan1=scan3|scan4|scan5|scan6|scan7;
				   };break;
			case 6:{					
					qt.scan0=scan0|scan1;
					qt.scan1=scan2|scan3|scan4|scan5|scan6|scan7;
				   };break;
			case 7:{					
					qt.scan0=scan0;
					qt.scan1=scan1|scan2|scan3|scan4|scan5|scan6|scan7;
				   };break;
			}

			return qt;
		}
		
		QCodeOp gOp;
		QCodeOp::QCodeOp()
		{
			QCodeOp::buildDownLevelTable();
		}
		void QCodeOp::buildDownLevelTable()
		{
			//-1 0
			{		
				u8 mk1= 0x3;	//00000011
				u8 mk2= mk1<<2; //00001100
				u8 mk3= mk1<<4; //00110000
				u8 mk4= mk1<<6;
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0x0)|((i&mk2)?0x2:0x0)|((i&mk3)?0x4:0x0)|((i&mk4)?0x8:0x0);
					QCodeOp::LEVEL_1_0[i] = v;
				}
			}
			//-1 1
			{				
				u8 mk1= 0x1;	//00000001
				u8 mk2= 0x3<<1; //00000110
				u8 mk3= 0x3<<3; //00011000
				u8 mk4= 0x3<<5; //01100000
				u8 mk5= 0x3<<7; //10000000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0x0)|((i&mk2)?0x2:0x0)|((i&mk3)?0x4:0x0)|((i&mk4)?0x8:0x0)|((i&mk5)?0x10:0x0);
					QCodeOp::LEVEL_1_1[i] = v;
				}
			}

			//------2------
			//-2 0
			{
				u8	mk1= 0xF;  //00001111
				u8  mk2= 0xF0; //11110000
				for(int i=0;i<256;i++)
				{
					u8 v= ((i&mk1)?0x1:0x0) | ((i&mk2)?0x2:0x0);
					QCodeOp::LEVEL_2_0[i] = v;
				}
			}			
			//-2 1
			{
				u8	mk1= 0x07;	//00000111
				u8  mk2= 0x71;  //01111000
				u8	mk3= 0x80;  //10000000
				for(int i=0;i<256;i++)
				{
					u8 v= ((i&mk1)?0x1:0x0) | ((i&mk2)?0x2:0x0)|((i&mk3)?0x4:0x0);
					QCodeOp::LEVEL_2_1[i] = v;
				}
			}
			//-2 2
			{
				u8	mk1= 0x03;	//00000011
				u8  mk2= 0x3C;  //00111100
				u8	mk3= 0xC0;  //11000000
				for(int i=0;i<256;i++)
				{
					u8 v= ((i&mk1)?0x1:0x0) | ((i&mk2)?0x2:0x0)|((i&mk3)?0x4:0x0);
					QCodeOp::LEVEL_2_2[i] = v;
				}
			}
			//-2 3
			{
				u8	mk1= 0x01;	//00000001
				u8  mk2= 0x1E;  //00011110
				u8	mk3= 0xE0;  //11100000
				for(int i=0;i<256;i++)
				{
					u8 v= ((i&mk1)?0x1:0x0) | ((i&mk2)?0x2:0x0)|((i&mk3)?0x4:0x0);
					QCodeOp::LEVEL_2_3[i] = v;
				}
			}

			//-3 0 
			{
				u8 mk1=0xFF; //11111111
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0);
					QCodeOp::LEVEL_3_0[i] = v;
				}
			}
			//-3 1 
			{
				u8 mk1=0x7F; //01111111
				u8 mk2=0x80; //10000000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_1[i] = v;
				}
			}
			//-3 2 
			{
				u8 mk1=0x3F; //00111111
				u8 mk2=0xC0; //11000000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_2[i] = v;
				}
			}
			//-3 3 
			{
				u8 mk1=0x1F; //00011111
				u8 mk2=0xE0; //11100000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_3[i] = v;
				}
			}
			//-3 4 
			{
				u8 mk1=0x0F; //00001111
				u8 mk2=0xF0; //11110000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_4[i] = v;
				}
			}
			//-3 5 
			{
				u8 mk1=0x07; //00000111
				u8 mk2=0xF8; //11111000
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_5[i] = v;
				}
			}
			
			//-3 6 
			{
				u8 mk1=0x03; //00000011
				u8 mk2=0xFC; //11111100
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_6[i] = v;
				}
			}
			
			//-3 7 
			{
				u8 mk1=0x01; //00000001
				u8 mk2=0xFE; //11111110
				for(int i=0;i<256;i++)
				{
					u8 v=((i&mk1)?0x1:0)|((i&mk2)?0x2:0);
					QCodeOp::LEVEL_3_7[i] = v;
				}
			}
		}
	
		void DrawQCode(GQIndexRasterizer* ras,GMapDrawing* pDrawing,QCODE& qindex,Color color){
		GQIndexRasterizer* rasterizer=ras;
		GRectangle2dd box = rasterizer->GetAdjustedBoundingBox();
		u32 dimension = rasterizer->GetLevelDimension(qindex.LEVEL);
		f64 cellSize = rasterizer->GetLevelPixelSize(qindex.LEVEL);
		Graphics* graphics = pDrawing->GetGraphics();
		Pen pen0(Color::DarkBlue);
		SolidBrush brush0(color);

		double area = box.GetWidth()*box.GetHeight();
		double area1=0;
		PointF points[4];
		Pen pen(Color(255,255,0,255));
		for(int iy=0;iy<8;iy++)
		{
			int iyy = qindex.Y+iy;
			f64 yy0= box.MinEdge.Y+iyy*cellSize;
			f64 yy1= yy0+cellSize;
			for(int ix=0;ix<8;ix++)
			{
				int ixx = qindex.X+ix;
				f64 xx0= box.MinEdge.X+ixx*cellSize;
				f64 xx1= xx0+cellSize;

				GVector2dd p0(xx0,yy0);
				GVector2dd p2(xx1,yy1);

				pDrawing->MapToView(p0,points[0]);
				pDrawing->MapToView(p2,points[2]);
				points[1]=PointF(points[2].X,points[0].Y);
				points[3]=PointF(points[0].X,points[2].Y);
				if(GQIndexRasterizer::GetIndexPixelFlag(&qindex,ix,iy))
				{
					graphics->FillPolygon(&brush0,points,4);
					graphics->DrawPolygon(&pen0,points,4);
					area1+=cellSize*cellSize;
				}	
			}
		}
		f64 yy0= box.MinEdge.Y+qindex.Y*cellSize;
		f64 yy1= yy0+cellSize*8;


		f64 xx0= box.MinEdge.X+qindex.X*cellSize;
		f64 xx1= xx0+cellSize*8;

		GVector2dd p0(xx0,yy0);
		GVector2dd p2(xx1,yy1);

		pDrawing->MapToView(p0,points[0]);
		pDrawing->MapToView(p2,points[2]);
		points[1]=PointF(points[2].X,points[0].Y);
		points[3]=PointF(points[0].X,points[2].Y);
	}

}

