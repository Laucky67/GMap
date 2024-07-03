#include "GMapView.h"
#include "GMap.h"
namespace gmap{

	GMapView::GMapView()
	{
		Map=NULL;
		CachedImage=NULL;
		CachedGraphics=NULL;
	}

	GMapView::~GMapView(void)
	{
		if(Map)Map->Release();
		if(CachedImage)
		{
			delete CachedImage;
			CachedImage = NULL;
		}
	}

	void GMapView::AttachMap(GMap* map)
	{
		if(Map)
		{
			Map->Release();
		}
			Map = map;
		if(Map)
			Map->AddRef();
	}
	//!设置视口
	void 	GMapView::OnResize(const RectF& viewport)
	{
		ViewPort = viewport;
		if(CachedImage)
		{
			delete CachedImage;
			CachedImage = NULL;
		}

	}

	//!得到真实的绘制句柄
	Graphics* GMapView::GetWindowGraphics()
	{
		return DrawingGraphics;
	}

	//!获得视口
	const RectF& 	GMapView::GetViewPort()
	{
		return ViewPort;
	}

	//!开始绘图
	void 	GMapView::BeginDraw(Graphics* graphics)
	{
		DrawingGraphics = graphics;
		if(CachedImage==NULL)
		{
			CachedImage = new Bitmap(ViewPort.Width,ViewPort.Height);
		}
		CachedGraphics = new Graphics(CachedImage);
		CachedGraphics->Clear(Color::White);
	}

	void GMapView::DrawCachedMap(Graphics* graphics,int offsetX,int offsetY)
	{
		if(CachedImage)
		{
			CachedBitmap* cb = new CachedBitmap(CachedImage,graphics);
			graphics->DrawCachedBitmap(cb,offsetX,offsetY);
			delete cb;
		}
	}
	void 	GMapView::DrawMap()
	{
		if(Map)
		{
			Map->Draw(this);
		}
	}

	//!结束绘图
	void 	GMapView::EndDraw()
	{		
		if(CachedGraphics)
		{
			delete CachedGraphics;
		}
		CachedBitmap* cb = new CachedBitmap(CachedImage,DrawingGraphics);
		DrawingGraphics->DrawCachedBitmap(cb,0,0);
		delete cb;
		DrawingGraphics = NULL;

	}

	//!视图到地图坐标
	void 	GMapView::ViewToMap(PointF& viewPoint,GCoordinate& mapPoint)
	{
		f64 mapx,mapy;
		ViewToMap(viewPoint.X,viewPoint.Y,mapx,mapy);
		mapPoint.X = mapx;
		mapPoint.Y = mapy;
	}

	//!视图到地图坐标
	void 	GMapView::ViewToMap(f32 viewX,f32 viewY,f64& mapX,f64& mapY)
	{
		mapX = ((double)viewX-(double)ViewPort.Width/2.0f)/Scale+CenterX;
		mapY = CenterY-((double)viewY-(double)ViewPort.Height/2.0f)/Scale;
	}

	////////////////////////////////////////////////////////
	//	GMapDrawing
	////////////////////////////////////////////////////////
	//!得到绘制的Gdi+对象
	Graphics* 	GMapView::GetGraphics()
	{
		return CachedGraphics;
	}

	//!得到包围盒
	const GRectangle2dd GMapView::GetViewBounds()
	{
		static GRectangle2dd rc;
	
		f64 x1,y1,x2,y2;
		ViewToMap(0,0,x1,y1);
		//rc.AddPoint(x,y);

		ViewToMap(ViewPort.Width,ViewPort.Height,x2,y2);
		//rc.AddPoint(x2,y2);

		GRectangle2dd rc2(x1,y1,x2,y2);

		rc2.Repair();

		return rc2;

	}

	//!得到缩放比
	f64		GMapView::GetScale()
	{
		return Scale;
	}

	//!从地图转到视图坐标
	void 	GMapView::MapToView(GCoordinate& coordinate,PointF& point)
	{
		MapToView(coordinate.X,coordinate.Y,point.X,point.Y);
	}

	//!从地图转到视图坐标
	void	GMapView:: MapToView(GCoordinate* pCoordinates,PointF* pPoints,int count,int* outPoint)
	{
		PointF lastPoint;
		
		if(count>16)
		{
			MapToView(pCoordinates[0],pPoints[0]);
			int nPointCount=1;
			PointF tPoint;
			for(int i=1;i<count-1;i++)
			{
				MapToView(pCoordinates[i],tPoint);
				if(abs(tPoint.X-lastPoint.X)+abs(tPoint.Y-lastPoint.Y)>2)
				{
					pPoints[nPointCount]=tPoint;
					lastPoint = tPoint;
					nPointCount++;
				}
			}
			MapToView(pCoordinates[count-1],pPoints[nPointCount]);
			nPointCount++;
			*outPoint = nPointCount;

		}
		else
		{
			for(int i=0;i<count;i++)
			{
				MapToView(pCoordinates[i],pPoints[i]);
			}
			*outPoint= count;
		}
	}

	void GMapView:: MapToView(f64 mapX,f64 mapY,f32& viewX,f32& viewY)
	{
		viewX = (mapX-CenterX)*Scale+ViewPort.Width/2.0f;
		viewY = ViewPort.Height/2.0f-(mapY-CenterY)*Scale;

	}

	//////////////////////////////////////////////////////////////
	//操作函数
	//////////////////////////////////////////////////////////////
	//!设置中心点
	void 	GMapView::SetCenter(f64 x,f64 y)
	{
		CenterX =x;
		CenterY =y;
	}

	//!得到中心点
	void 	GMapView::GetCenter(f64& x,f64& y)
	{
		x = CenterX;
		y = CenterY;
	}

	//!设置缩放比
	void 	GMapView::SetScale(f64 scale)
	{
		Scale=scale;
	}

	//!缩放
	void 	GMapView::Zoom(f64 factor)
	{
		Scale*=factor;
	}

	//!全屏
	void 	GMapView::ToMapExtent()
	{
		GRectangle2dd rc;
		Map->GetBoundingBox(rc);

		ToExtent(rc);
	}

	//!设置范围
	void 	GMapView::ToExtent(GRectangle2dd& rc)
	{
		f64 centerX = (rc.MinEdge.X+rc.MaxEdge.X)/2;
		f64 centerY = (rc.MinEdge.Y+rc.MaxEdge.Y)/2;

		f64 scale0 = ViewPort.Width/(rc.MaxEdge.X -rc.MinEdge.X);
		f64 scale1 = ViewPort.Height/(rc.MaxEdge.Y-rc.MinEdge.Y);

		CenterX = centerX;
		CenterY = centerY;

		Scale = scale0<scale1?scale0:scale1;
	}

}

