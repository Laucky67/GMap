#include "GDefaultUITool.h"
#include "tchar.h"
namespace gmap{

	GDefaultUITool::GDefaultUITool(IMapUIContext* context):
GMapUITool(context)
{
	LeftDown = false;
}

GDefaultUITool::~GDefaultUITool(void)
{

}

bool GDefaultUITool::OnMouseMove(s32 x,s32 y,s32 flags)
{
	f64 x1,y1,x0,y0;
	if(LeftDown)
	{

		s32 dx=x-MouseDownX;
		s32 dy=y-MouseDownY;

		MapUIContext->DrawCachedMap(dx,dy);
	}
	else
	{	
		wchar_t buffer[500];			
		MapUIContext->GetView()->ViewToMap(x,y,x0,y0);

		_sntprintf(buffer,500,L"x=%f,y=%f",x0,y0);

		MapUIContext->SetStatusText(0,wstring(buffer));

	}
	return true;
}

bool GDefaultUITool::OnMouseDown(u32 x,s32 y,s32 button)
{
	MouseDownX= LastDownX = x;
	MouseDownY = LastDownY = y;
	if(button == MK_LBUTTON)
	{
		LeftDown = true;
	}
	return true;
}

bool GDefaultUITool::OnMouseUp(s32 x,s32 y,s32 button)
{
	f64 x1,y1,x0,y0;
	if(LeftDown)
	{

		MapUIContext->GetView()->ViewToMap(x,y,x1,y1);

		MapUIContext->GetView()->ViewToMap(MouseDownX,MouseDownY,x0,y0);

		f64 centerX,centerY;
		MapUIContext->GetView()->GetCenter(centerX,centerY);
		centerX-=(x1-x0);
		centerY-=(y1-y0);
		MapUIContext->GetView()->SetCenter(centerX,centerY);


		MapUIContext->Redraw();
	}
	LeftDown = false;
	return true;
}

bool GDefaultUITool::OnMouseWheel(s32 x,s32 y,u32 button,s32 delta)
{
	f64 scale = MapUIContext->GetView()->GetScale();
	if(delta>0)
	{

		MapUIContext->GetView()->SetScale(scale*2);
	}
	else
	{
		MapUIContext->GetView()->SetScale(scale/2);
	}
	MapUIContext->Redraw();
	return true;
}
}
