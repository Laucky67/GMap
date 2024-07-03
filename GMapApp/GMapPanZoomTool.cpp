#include "stdafx.h"
#include "GMapPanZoomTool.h"
#include "GMapDrawingImpl.h"
namespace gmap
{

	GMapPanZoomTool::GMapPanZoomTool(IMapUIContext* context) :
		GMapUITool(context)
	{
		LeftDown = false;
	}

	GMapPanZoomTool::~GMapPanZoomTool(void)
	{
	}

	bool GMapPanZoomTool::OnMouseMove(s32 x, s32 y, s32 flags)
	{
		f64 x1, y1, x0, y0;
		if (LeftDown)
		{
			s32 dx = x - MouseDownX;
			s32 dy = y - MouseDownY;
			MapUIContext->DrawCache(dx, dy);
		}
		else
		{
			wchar_t buffer[500];
			MapUIContext->GetDrawing()->ViewToMap(x, y, x0, y0);
			_sntprintf(buffer, 500, L"x=%f,y=%f", x0, y0);
			MapUIContext->SetStatusText(0, wstring(buffer));
		}
		return true;
	}

	bool GMapPanZoomTool::OnMouseDown(u32 x, s32 y, s32 button)
	{
		MouseDownX = LastDownX = x;
		MouseDownY = LastDownY = y;
		if (button == MK_LBUTTON)
		{
			LeftDown = true;
		}
		return true;
	}

	bool GMapPanZoomTool::OnMouseUp(s32 x, s32 y, s32 button)
	{
		f64 x1, y1, x0, y0;
		if (LeftDown)
		{
			MapUIContext->GetDrawing()->ViewToMap(x, y, x1, y1);
			MapUIContext->GetDrawing()->ViewToMap(MouseDownX, MouseDownY, x0, y0);
			f64 centerX, centerY;
			static_cast<GMapDrawingImpl*>(MapUIContext->GetDrawing())->GetCenter(centerX, centerY);
			centerX -= (x1 - x0);
			centerY -= (y1 - y0);
			static_cast<GMapDrawingImpl*>(MapUIContext->GetDrawing())->SetCenter(centerX, centerY);
			MapUIContext->Redraw();
		}
		LeftDown = false;
		return true;
	}

	bool GMapPanZoomTool::OnMouseWheel(s32 x, s32 y, u32 button, s32 delta)
	{
		f64 scale = MapUIContext->GetDrawing()->GetScale();
		if (delta > 0)
		{
			static_cast<GMapDrawingImpl*>(MapUIContext->GetDrawing())->SetScale(scale * 2);
		}
		else
		{
			static_cast<GMapDrawingImpl*>(MapUIContext->GetDrawing())->SetScale(scale / 2);
		}
		MapUIContext->Redraw();
		return true;
	}
}