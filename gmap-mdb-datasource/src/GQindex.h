#ifndef __G_GEOMETRY_RASTERIZER_H__
#define __G_GEOMETRY_RASTERIZER_H__

#include "GCoreBase.h"
#include "GGeometry.h"
#include "GLineString.h"
#include "GPolygon.h"
#include "GMultiGeometry.h"

namespace Gdiplus
{
	class Bitmap;
	class Graphics;
	class Pen;
	class Brush;
	class Point;
};

namespace gmap
{
	//!
	struct GQINDEX
	{
		u32 LEVEL:8;
		u32 X:24;
		u32 Y:24;
		union{
			unsigned __int64 IMAGE;
			struct
			{
				u8	scan0;
				u8  scan1;
				u8  scan2;
				u8	scan3;				
				u8	scan4;
				u8  scan5;
				u8  scan6;
				u8	scan7;
			};
		};
	};

	typedef GQINDEX	QCODE;

	class QCodeOp
	{
	public:
		QCodeOp();

		static QCODE Align(QCODE q1,QCODE q2);

		//!盒子是否相交
		static bool IntersectBox( QCODE q1, QCODE q2);

		//!Bitmap是否相交
		static bool IntersectBitmap( QCODE q1, QCODE q2);

		//!是否相交
		static bool Intersect(const QCODE q1,const QCODE q2);

		static QCODE DownL(const QCODE& q,int level);
		
		//!降级1
		static QCODE DownL1(const QCODE& q);

		//!降级2
		static QCODE DownL2(const QCODE& q);

		//!降级3
		static QCODE DownL3(const QCODE& q,int level=3);

	public:
		static u8 LEVEL_1_0[256];
		static u8 LEVEL_1_1[256];

		static u8 LEVEL_2_0[256];
		static u8 LEVEL_2_1[256];
		static u8 LEVEL_2_2[256];
		static u8 LEVEL_2_3[256];

		
		static u8 LEVEL_3_0[256];
		static u8 LEVEL_3_1[256];
		static u8 LEVEL_3_2[256];
		static u8 LEVEL_3_3[256];
		static u8 LEVEL_3_4[256];
		static u8 LEVEL_3_5[256];
		static u8 LEVEL_3_6[256];
		static u8 LEVEL_3_7[256];

		static void buildDownLevelTable();

	};
	//!几何对象光栅化器
	class GQIndexRasterizer
	{
	public:
		//!构造函数
		GQIndexRasterizer(void);

		//!析构函数
		~GQIndexRasterizer(void);

		//!初始化
		void Initialize(const GRectangle2dd& bbox);

		//!结束
		void Uninitialize();

		//!得到修正过的包围盒
		const GRectangle2dd GetAdjustedBoundingBox();

		//!得到不同层次单元尺寸
		f64	   GetLevelPixelSize(u32 level);

		//!得到大小
		u32    GetLevelDimension(u32 level);

		//!光栅化
		GQINDEX  RasterizeGeometry(GGeometry* pGeometry);

		//!static
		static bool GetIndexPixelFlag(GQINDEX* pQIndex,u8 x,u8 y);

		//!设置像素索引标记
		static void SetIndexPixelFlag(GQINDEX* pQIndex,u8 x,u8 y,bool flag);
	private:
		inline void RasterizeGeometry(Gdiplus::Graphics* graphics,const GVector2dd& minEdge,GLineString* lineString,f64 d);

		inline void RasterizeGeometry(Gdiplus::Graphics* graphics,const GVector2dd& minEdge,GPolygon* polygon,f64 d);
	private:
		Gdiplus::Bitmap*	CachedBitmap;
		Gdiplus::Graphics*  CachedGraphics;
		Gdiplus::Point*		CachedPoints;
		u32					CachedPointCount;
		Gdiplus::Pen*				CachedPen;
		Gdiplus::Brush*				CachedBrush;    
		GRectangle2dd				AdjustedBBox;
		GVector2dd					BBoxMinEdge;
		GVector2dd					BBoxDimension;

	};
	class GMapDrawing;
	void DrawQCode(GQIndexRasterizer* ras,GMapDrawing* pDrawing,QCODE& qindex,Color color= Color(100,0,0,0));
}

#endif