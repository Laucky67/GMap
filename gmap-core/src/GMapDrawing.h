#ifndef __G_MAP_DRAWING_H__
#define __G_MAP_DRAWING_H__

#include "GGeometry.h"
namespace gmap
{
	//!��ͼ�����ӿ�
	class GMapDrawing
	{
	public:
		//!�õ����Ƶ�Gdi+����
		virtual Graphics*			GetGraphics()=0;
		//!�õ���Χ��
		virtual const GRectangle2dd GetViewBounds()=0;
		//!����ӿ�
		virtual const RectF&		GetViewPort()=0;
		//!�õ����ű�
		virtual f64					GetScale()=0;
		//!�ӵ�ͼת����ͼ����
		virtual void				MapToView(GCoordinate& coordinate,PointF& point)=0;
		//!�ӵ�ͼת����ͼ����
		virtual void				MapToView(GCoordinate* pCoordinates,PointF* pPoints,int count,int* outPoint)=0;
		//!�ӵ�ͼת����ͼ����
		virtual void				MapToView(f64 mapX,f64 mapY,f32& viewX,f32& viewY)=0;
		//!��ͼ����ͼ����
		virtual void				ViewToMap(PointF& viewPoint, GCoordinate& mapPoint)=0;
		//!��ͼ����ͼ����
		virtual void				ViewToMap(f32 viewX, f32 viewY, f64& mapX, f64& mapY)=0;
	};
}
#endif