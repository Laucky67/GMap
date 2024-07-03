#include "GPointEditObject.h"
#include "GHitTestHelper.h"
#include "GMapDrawing.h"

namespace gmap{

	GPointEditObject::GPointEditObject(const wstring& name,GPoint* point)
		:GMapEditObject(name)
	{
		Point = point;
		Point->AddRef();
	}

	GPointEditObject::~GPointEditObject(void)
	{
		Point->Release();
	}

	//!ִ�в���
	bool GPointEditObject::HitTest(GVector2dd& hitPoint,f64 tolerence)
	{
		return GHitTestHelper::GetDistance(Point,hitPoint)<tolerence*10;
	}

	//!��ѯ��ǰλ����֧�ֱ༭����
	GMapEditObject::EnumEditHandleType GPointEditObject::QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence)
	{
		if(GHitTestHelper::GetDistance(Point,hitPoint)<tolerence)
		{
			*pHandleCookie = 0;
			return GMapEditObject::EHT_MOVE_HANDLE;
		}
		else
		{
			return GMapEditObject::EHT_NONE;
		}
	}

	void GPointEditObject::DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		GCoordinate coord ;
		PointF		point;	
		Point->GetCoordinate(&coord);
		MapUIContext->GetDrawing()->MapToView(coord,point);
		
		Pen pen(Color::Red);
		SolidBrush brush(Color(123,255,0,0));

		graphics->DrawEllipse(&pen,RectF(point.X-20,point.Y-20,40,40));
		graphics->FillEllipse(&brush,RectF(point.X-20,point.Y-20,40,40));
	}

	//!��ʼ�༭
	void GPointEditObject::BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		Editing =true;		
		EditGraphics = graphics;
	}

	//!�������
	void GPointEditObject::MouseDown(GVector2dd& point,u32 flags)
	{
		
	}

	//!����ƶ�
	void GPointEditObject::MouseMove(GVector2dd& point,u32 flags)
	{
		if(Editing)
		{
			MapUIContext->DrawCache();
			Point->SetCoordinate(point);
			Draw(EditGraphics);
		}

	}

	//!���̧��
	void GPointEditObject::MouseUp(GVector2dd& point,u32 flags)
	{

	}

	//!�����༭
	void GPointEditObject::EndEdit()
	{
		Editing = false;
		EditGraphics = NULL;
		MapUIContext->ReleaseEditGraphics();
	}

	//!��������
	void GPointEditObject::Draw(Graphics* graphics)
	{
		GCoordinate coord ;
		PointF		point;	
		Point->GetCoordinate(&coord);
		MapUIContext->GetDrawing()->MapToView(coord,point);
		
		Pen pen(Color::Red);
		SolidBrush brush(Color(123,255,0,255));

		graphics->DrawEllipse(&pen,RectF(point.X-10,point.Y-10,20,20));
		graphics->FillEllipse(&brush,RectF(point.X-10,point.Y-10,20,20));
	}

	//!�޸�֮��
	void GPointEditObject::OnEdited()
	{

	}

}
