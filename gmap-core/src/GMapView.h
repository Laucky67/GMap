#ifndef __G_MAP_VIEW_H__
#define __G_MAP_VIEW_H__

#include "GCoreBase.h"
#include "GMapDrawing.h"
namespace gmap{

	class GMap;
	class GMAPCORE_API GMapView:public GUnknown,public GMapDrawing
	{
	public:
		GMapView();

		~GMapView(void);

		void AttachMap(GMap* map);

		GMap* GetMap(){return Map;}

		//!设置视口
		void OnResize(const RectF& viewport);

		//!获得视口
		const RectF& GetViewPort();

		void DrawCachedMap(Graphics* graphics,int offsetX=0,int offsetY=0);

		//!开始绘图
		void BeginDraw(Graphics* graphics);
		
		void DrawMap();

		//!结束绘图
		void EndDraw();

		//!视图到地图坐标
		void ViewToMap(PointF& viewPoint,GCoordinate& mapPoint);

		//!视图到地图坐标
		void ViewToMap(f32 viewX,f32 viewY,f64& mapX,f64& mapY);

		////////////////////////////////////////////////////////
		//	GMapDrawing
		////////////////////////////////////////////////////////
		//!得到绘制的Gdi+对象
		virtual Graphics* GetGraphics();

		//!得到真实的绘制句柄
		virtual Graphics* GetWindowGraphics();

		//!得到包围盒
		virtual const GRectangle2dd GetViewBounds();

		//!得到缩放比
		virtual f64	GetScale();

		//!从地图转到视图坐标
		virtual void MapToView(GCoordinate& coordinate,PointF& point);

		//!从地图转到视图坐标
		virtual void MapToView(GCoordinate* pCoordinates,PointF* pPoints,int count,int* outPoint);

		//!从地图转到视图坐标
		virtual void MapToView(f64 mapX,f64 mapY,f32& viewX,f32& viewY);

		//////////////////////////////////////////////////////////////
		//操作函数
		//////////////////////////////////////////////////////////////
		//!设置中心点
		void SetCenter(f64 x,f64 y);

		//!得到中心点
		void GetCenter(f64& x,f64& y);

		//!设置缩放比
		void SetScale(f64 scale);

		//!缩放
		void Zoom(f64 factor);

		//!全屏
		void ToMapExtent();

		//!设置范围
		void ToExtent(GRectangle2dd& rc);

	private:
		f64				Scale;
		f64				CenterX;
		f64				CenterY;

		RectF			ViewPort;
		GRectangle2dd	ViewBounds;
		Graphics*		DrawingGraphics;
		GMap*			Map;		
		Bitmap*			CachedImage;
		Graphics*		CachedGraphics;
	};

}

#endif