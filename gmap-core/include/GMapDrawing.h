#ifndef __G_MAP_DRAWING_H__
#define __G_MAP_DRAWING_H__

#include "GGeometry.h"
namespace gmap
{
	//!地图参数接口
	class GMapDrawing
	{
	public:
		//!得到绘制的Gdi+对象
		virtual Graphics*			GetGraphics()=0;
		//!得到包围盒
		virtual const GRectangle2dd GetViewBounds()=0;
		//!获得视口
		virtual const RectF&		GetViewPort()=0;
		//!得到缩放比
		virtual f64					GetScale()=0;
		//!从地图转到视图坐标
		virtual void				MapToView(GCoordinate& coordinate,PointF& point)=0;
		//!从地图转到视图坐标
		virtual void				MapToView(GCoordinate* pCoordinates,PointF* pPoints,int count,int* outPoint)=0;
		//!从地图转到视图坐标
		virtual void				MapToView(f64 mapX,f64 mapY,f32& viewX,f32& viewY)=0;
		//!视图到地图坐标
		virtual void				ViewToMap(PointF& viewPoint, GCoordinate& mapPoint)=0;
		//!视图到地图坐标
		virtual void				ViewToMap(f32 viewX, f32 viewY, f64& mapX, f64& mapY)=0;
	};
}
#endif