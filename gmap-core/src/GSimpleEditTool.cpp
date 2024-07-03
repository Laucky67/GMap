#include "GSimpleEditTool.h"
namespace gmap
{
	const double TOL = 5.0;
	GSimpleEditTool::GSimpleEditTool(IMapUIContext * context)
		:GMapUITool(context)
	{
		CurrentEditObject = NULL;
		Editing = false;
	}
	GSimpleEditTool::~GSimpleEditTool()
	{
		if (CurrentEditObject)
		{
			CurrentEditObject->Release();
		}
	}
	bool GSimpleEditTool::OnMouseMove(s32 x, s32 y, s32 flags) {
		f64 mapX, mapY;
		MapUIContext->GetDrawing()->ViewToMap(x, y, mapX, mapY);
		GCoordinate hitPoint(mapX, mapY);
		f32 tolerence = TOL / MapUIContext->GetDrawing()->GetScale();
		if (CurrentEditObject != NULL)
		{
			if (!Editing)
			{
				u32 hitCookie = 0;
				GMapEditObject::EnumEditHandleType handleType = CurrentEditObject->QueryEditHandle(hitPoint, &hitCookie, tolerence);
				if (handleType != GMapEditObject::EHT_NONE)
				{
					MapUIContext->DrawCache();
					Graphics* pEditGraphics = MapUIContext->GetEditGraphics();
					CurrentEditObject->Draw(pEditGraphics);
					CurrentEditObject->DrawEditEffect(pEditGraphics, handleType, hitPoint, hitCookie);
					MapUIContext->ReleaseEditGraphics();
				}
			}
			else
			{
				CurrentEditObject->MouseMove(hitPoint, flags);
			}
		}
		return true;
	}
	bool GSimpleEditTool::OnMouseDown(u32 x, s32 y, s32 button)
	{
		f64 mapX, mapY;
		MapUIContext->GetDrawing()->ViewToMap(x, y, mapX, mapY);
		GCoordinate hitPoint(mapX, mapY);
		f32 tolerence = TOL / MapUIContext->GetDrawing()->GetScale();

		if (CurrentEditObject != NULL)
		{
			if (!Editing && (button&MK_LBUTTON) != NULL)
			{
				u32 hitCookie = 0;
				GMapEditObject::EnumEditHandleType  handleType = CurrentEditObject->QueryEditHandle(hitPoint, &hitCookie, tolerence);
				if (handleType != GMapEditObject::EHT_NONE)
				{
					Graphics* pEditGraphics = MapUIContext->GetEditGraphics();
					CurrentEditObject->BeginEdit(pEditGraphics, handleType, hitPoint, hitCookie);
					Editing = true;
					CurrentEditObject->MouseDown(hitPoint, button);
				}
			}
		}
		return true;
	}
	bool GSimpleEditTool::OnMouseUp(s32 x, s32 y, s32 button)
	{
		f64 mapX, mapY;
		MapUIContext->GetDrawing()->ViewToMap(x, y, mapX, mapY);
		GCoordinate hitPoint(mapX, mapY);
		f32 tolerence = TOL / MapUIContext->GetDrawing()->GetScale();

		if (CurrentEditObject != NULL)
		{
			if (Editing)
			{
				CurrentEditObject->MouseUp(hitPoint, button);
				CurrentEditObject->EndEdit();
				MapUIContext->ReleaseEditGraphics();
				Editing = false;
			}
			else
			{
				if (!CurrentEditObject->HitTest(hitPoint, tolerence))
				{
					CurrentEditObject->Release();
					CurrentEditObject = NULL;

					GMapLayer* pLayer = MapUIContext->GetMap()->GetCurrentLayer();
					if (pLayer != NULL)
					{
						GMapEditObject* pMapEditObject = pLayer->HitTest(hitPoint, tolerence);
						if (pMapEditObject)
						{
							CurrentEditObject = pMapEditObject;
							CurrentEditObject->AttachUIContext(MapUIContext);
						}
					}
				}
			}
		}
		else
		{
			GMapLayer* pLayer = MapUIContext->GetMap()->GetCurrentLayer();
			if (pLayer != NULL)
			{
				GMapEditObject* pMapEditObject = pLayer->HitTest(hitPoint, tolerence*10);
				if (pMapEditObject)
				{
					CurrentEditObject = pMapEditObject;
					CurrentEditObject->AttachUIContext(MapUIContext);
				}
			}
		}

		MapUIContext->Redraw();
		return true;
	}
	bool GSimpleEditTool::OnKeyDown(u32 key) { return true; }
	bool GSimpleEditTool::OnKeyUp(u32 key) { return true; }
	bool GSimpleEditTool::OnMouseWheel(s32 x, s32 y, u32 button, s32 delta) { return true; }
	void GSimpleEditTool::Draw(Graphics * graphics) {
		if (CurrentEditObject)
		{
			CurrentEditObject->Draw(graphics);
		}
	}
}
