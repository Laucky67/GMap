#pragma once
#include "gmap-include.h"
class GMapDrawingImpl:
	public GMapDrawing
{
public:
	GMapDrawingImpl();
	~GMapDrawingImpl();
	//!��ʼ����
	void	BeginDraw(Graphics* g);
	//!��������
	void	EndDraw();
	//!�����ӿ�
	void	Resize(const RectF& viewport);
	//!�������ĵ�
	void	SetCenter(f64 x, f64 y);
	//!�õ����ĵ�
	void	GetCenter(f64& x, f64& y);
	//!�������ű�
	void	SetScale(f64 scale);
	//!����
	void	Zoom(f64 factor);
	//!���÷�Χ
	void	ToExtent(GRectangle2dd& rc);
public:
	//!�õ����Ƶ�Gdi+����
	virtual Graphics*			GetGraphics();
	//!�õ���Χ��
	virtual const GRectangle2dd GetViewBounds();
	//!����ӿ�
	virtual const RectF&		GetViewPort();
	//!�õ����ű�
	virtual f64					GetScale();
	//!�ӵ�ͼת����ͼ����
	virtual void				MapToView(GCoordinate& coordinate, PointF& point);
	//!�ӵ�ͼת����ͼ����
	virtual void				MapToView(GCoordinate* pCoordinates, PointF* pPoints, int count, int* outPoint);
	//!�ӵ�ͼת����ͼ����
	virtual void				MapToView(f64 mapX, f64 mapY, f32& viewX, f32& viewY);
	//!��ͼ����ͼ����
	virtual void				ViewToMap(PointF& viewPoint, GCoordinate& mapPoint);
	//!��ͼ����ͼ����
	virtual void				ViewToMap(f32 viewX, f32 viewY, f64& mapX, f64& mapY);
private:
	f64				Scale;
	f64				CenterX;
	f64				CenterY;

	RectF			ViewPort;
	GRectangle2dd	ViewBounds;
	Graphics*		DrawingGraphics;
};

