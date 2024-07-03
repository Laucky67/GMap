#include "GMapDrawingImpl.h"

//!�����ӿ�
void 	GMapDrawingImpl::Resize(const RectF& viewport)
{
	ViewPort = viewport;
}

//!�õ���ʵ�Ļ��ƾ��
Graphics* GMapDrawingImpl::GetGraphics()
{
	return DrawingGraphics;
}

//!����ӿ�
const RectF& 	GMapDrawingImpl::GetViewPort()
{
	return ViewPort;
}

GMapDrawingImpl::GMapDrawingImpl()
{
	CenterX = CenterY = 0;
	Scale = -1;
}
GMapDrawingImpl::~GMapDrawingImpl()
{
}

//!��ʼ��ͼ
void 	GMapDrawingImpl::BeginDraw(Graphics* graphics)
{
	DrawingGraphics = graphics;
}


//!������ͼ
void 	GMapDrawingImpl::EndDraw()
{

}

//!��ͼ����ͼ����
void 	GMapDrawingImpl::ViewToMap(PointF& viewPoint, GCoordinate& mapPoint)
{
	f64 mapx, mapy;
	ViewToMap(viewPoint.X, viewPoint.Y, mapx, mapy);
	mapPoint.X = mapx;
	mapPoint.Y = mapy;
}

//!��ͼ����ͼ����
void 	GMapDrawingImpl::ViewToMap(f32 viewX, f32 viewY, f64& mapX, f64& mapY)
{
	mapX = ((double)viewX - (double)ViewPort.Width / 2.0f) / Scale + CenterX;
	mapY = CenterY - ((double)viewY - (double)ViewPort.Height / 2.0f) / Scale;
}
//!��ͼ����ͼ����
void GMapDrawingImpl::MapToView(f64 mapX, f64 mapY, f32& viewX, f32& viewY)
{
	viewX = (mapX - CenterX)*Scale + ViewPort.Width / 2.0f;
	viewY = ViewPort.Height / 2.0f - (mapY - CenterY)*Scale;

}

//!�õ���Χ��
const GRectangle2dd GMapDrawingImpl::GetViewBounds()
{
	static GRectangle2dd rc;

	f64 x1, y1, x2, y2;
	ViewToMap(0, 0, x1, y1);
	//rc.AddPoint(x,y);

	ViewToMap(ViewPort.Width, ViewPort.Height, x2, y2);
	//rc.AddPoint(x2,y2);

	GRectangle2dd rc2(x1, y1, x2, y2);

	rc2.Repair();

	return rc2;

}

//!�õ����ű�
f64		GMapDrawingImpl::GetScale()
{
	return Scale;
}

//!�ӵ�ͼת����ͼ����
void 	GMapDrawingImpl::MapToView(GCoordinate& coordinate, PointF& point)
{
	MapToView(coordinate.X, coordinate.Y, point.X, point.Y);
}

//!�ӵ�ͼת����ͼ����
void	GMapDrawingImpl::MapToView(GCoordinate* pCoordinates, PointF* pPoints, int count, int* outPoint)
{
	PointF lastPoint;

	if (count > 16)
	{
		MapToView(pCoordinates[0], pPoints[0]);
		int nPointCount = 1;
		PointF tPoint;
		for (int i = 1; i < count - 1; i++)
		{
			MapToView(pCoordinates[i], tPoint);
			if (abs(tPoint.X - lastPoint.X) + abs(tPoint.Y - lastPoint.Y) > 2)
			{
				pPoints[nPointCount] = tPoint;
				lastPoint = tPoint;
				nPointCount++;
			}
		}
		MapToView(pCoordinates[count - 1], pPoints[nPointCount]);
		nPointCount++;
		*outPoint = nPointCount;

	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			MapToView(pCoordinates[i], pPoints[i]);
		}
		*outPoint = count;
	}
}


//////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////
//!�������ĵ�
void 	GMapDrawingImpl::SetCenter(f64 x, f64 y)
{
	CenterX = x;
	CenterY = y;
}

//!�õ����ĵ�
void 	GMapDrawingImpl::GetCenter(f64& x, f64& y)
{
	x = CenterX;
	y = CenterY;
}

//!�������ű�
void 	GMapDrawingImpl::SetScale(f64 scale)
{
	Scale = scale;
}

//!����
void 	GMapDrawingImpl::Zoom(f64 factor)
{
	Scale *= factor;
}

//!���÷�Χ
void 	GMapDrawingImpl::ToExtent(GRectangle2dd& rc)
{
	f64 centerX = (rc.MinEdge.X + rc.MaxEdge.X) / 2;
	f64 centerY = (rc.MinEdge.Y + rc.MaxEdge.Y) / 2;

	f64 scale0 = ViewPort.Width / (rc.MaxEdge.X - rc.MinEdge.X);
	f64 scale1 = ViewPort.Height / (rc.MaxEdge.Y - rc.MinEdge.Y);

	CenterX = centerX;
	CenterY = centerY;

	Scale = scale0 < scale1 ? scale0 : scale1;
}