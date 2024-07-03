#pragma once
#include "gmap-include.h"
class GMapDrawingImpl:
	public GMapDrawing
{
public:
	GMapDrawingImpl();
	~GMapDrawingImpl();
	//!开始绘制
	void	BeginDraw(Graphics* g);
	//!结束绘制
	void	EndDraw();
	//!设置视口
	void	Resize(const RectF& viewport);
	//!设置中心点
	void	SetCenter(f64 x, f64 y);
	//!得到中心点
	void	GetCenter(f64& x, f64& y);
	//!设置缩放比
	void	SetScale(f64 scale);
	//!缩放
	void	Zoom(f64 factor);
	//!设置范围
	void	ToExtent(GRectangle2dd& rc);
public:
	//!得到绘制的Gdi+对象
	virtual Graphics*			GetGraphics();
	//!得到包围盒
	virtual const GRectangle2dd GetViewBounds();
	//!获得视口
	virtual const RectF&		GetViewPort();
	//!得到缩放比
	virtual f64					GetScale();
	//!从地图转到视图坐标
	virtual void				MapToView(GCoordinate& coordinate, PointF& point);
	//!从地图转到视图坐标
	virtual void				MapToView(GCoordinate* pCoordinates, PointF* pPoints, int count, int* outPoint);
	//!从地图转到视图坐标
	virtual void				MapToView(f64 mapX, f64 mapY, f32& viewX, f32& viewY);
	//!视图到地图坐标
	virtual void				ViewToMap(PointF& viewPoint, GCoordinate& mapPoint);
	//!视图到地图坐标
	virtual void				ViewToMap(f32 viewX, f32 viewY, f64& mapX, f64& mapY);
private:
	f64				Scale;
	f64				CenterX;
	f64				CenterY;

	RectF			ViewPort;
	GRectangle2dd	ViewBounds;
	Graphics*		DrawingGraphics;
};

