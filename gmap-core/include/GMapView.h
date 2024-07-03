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

		//!�����ӿ�
		void OnResize(const RectF& viewport);

		//!����ӿ�
		const RectF& GetViewPort();

		void DrawCachedMap(Graphics* graphics,int offsetX=0,int offsetY=0);

		//!��ʼ��ͼ
		void BeginDraw(Graphics* graphics);
		
		void DrawMap();

		//!������ͼ
		void EndDraw();

		//!��ͼ����ͼ����
		void ViewToMap(PointF& viewPoint,GCoordinate& mapPoint);

		//!��ͼ����ͼ����
		void ViewToMap(f32 viewX,f32 viewY,f64& mapX,f64& mapY);

		////////////////////////////////////////////////////////
		//	GMapDrawing
		////////////////////////////////////////////////////////
		//!�õ����Ƶ�Gdi+����
		virtual Graphics* GetGraphics();

		//!�õ���ʵ�Ļ��ƾ��
		virtual Graphics* GetWindowGraphics();

		//!�õ���Χ��
		virtual const GRectangle2dd GetViewBounds();

		//!�õ����ű�
		virtual f64	GetScale();

		//!�ӵ�ͼת����ͼ����
		virtual void MapToView(GCoordinate& coordinate,PointF& point);

		//!�ӵ�ͼת����ͼ����
		virtual void MapToView(GCoordinate* pCoordinates,PointF* pPoints,int count,int* outPoint);

		//!�ӵ�ͼת����ͼ����
		virtual void MapToView(f64 mapX,f64 mapY,f32& viewX,f32& viewY);

		//////////////////////////////////////////////////////////////
		//��������
		//////////////////////////////////////////////////////////////
		//!�������ĵ�
		void SetCenter(f64 x,f64 y);

		//!�õ����ĵ�
		void GetCenter(f64& x,f64& y);

		//!�������ű�
		void SetScale(f64 scale);

		//!����
		void Zoom(f64 factor);

		//!ȫ��
		void ToMapExtent();

		//!���÷�Χ
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