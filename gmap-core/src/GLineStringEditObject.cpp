#include "GLineStringEditObject.h"
#include "GHitTestHelper.h"
#include "GMapDrawing.h"

namespace gmap{

	GLineStringEditObject::GLineStringEditObject(const wstring& name,GLineString* lineString)
		:GMapEditObject(name)
	{
		LineString = lineString;
		LineString->AddRef();
	}


	GLineStringEditObject::~GLineStringEditObject(void)
	{
		LineString->Release();
	}

	//!执行测试
	bool GLineStringEditObject::HitTest(GVector2dd& hitPoint,f64 tolerence)
	{
		u32 index;
		f64 d1=GHitTestHelper::GetNearestPoint(LineString,hitPoint,&index,tolerence);

		if(d1<tolerence)
		{
			return true;
		}
		else
		{
			f64 d2=GHitTestHelper::GetNearestEdge(LineString,hitPoint,&index,false,tolerence);
		}
		return false;
	}

	//!查询当前位置上支持编辑类型
	GMapEditObject::EnumEditHandleType GLineStringEditObject::QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence)
	{
		u32 index;
		f64 d1=GHitTestHelper::GetNearestPoint(LineString,hitPoint,&index,tolerence);
		if(d1<tolerence)
		{
			*pHandleCookie = index;
			return GMapEditObject::EHT_NODE_HANDLE;
		}
		else
		{
			d1=GHitTestHelper::GetNearestEdge(LineString,hitPoint,&index,false,tolerence);
			if(d1<tolerence)
			{
				*pHandleCookie = index;
				return GMapEditObject::EHT_EDGE_HANDLE; 
			}
		}
		
		return GMapEditObject::EHT_NONE;	
	}


	void GLineStringEditObject::DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		Pen pen(Color::Red,2.0);

		if(handlerType==GMapEditObject::EHT_NODE_HANDLE)
		{
			GCoordinate coord = LineString->GetCoordinateAt(handleCookie);
			PointF point;
			MapUIContext->GetDrawing()->MapToView(coord,point);

			graphics->DrawRectangle(&pen,RectF(point.X-4,point.Y-4,8,8));

		}
		else if(handlerType==GMapEditObject::EHT_EDGE_HANDLE)
		{	
			if(handleCookie>LineString->GetCoordinateCount()-1)
			{
				int i=0;
				return;
			}
			GCoordinate coord1 = LineString->GetCoordinateAt(handleCookie);

			GCoordinate coord2 = LineString->GetCoordinateAt(handleCookie+1);
			PointF point1,point2;
			MapUIContext->GetDrawing()->MapToView(coord1,point1);
			MapUIContext->GetDrawing()->MapToView(coord2,point2);

			graphics->DrawLine(&pen,point1,point2);
		}
	}

	//!开始编辑
	void GLineStringEditObject::BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		EditHandleType = handlerType;
		HandleCookie =	 handleCookie;
		EditGraphics=	 graphics;
	}

	//!鼠标落下
	void GLineStringEditObject::MouseDown(GVector2dd& point,u32 flags){
		
		if(MK_LBUTTON&flags)
		{
			if(EditHandleType==GMapEditObject::EHT_NODE_HANDLE)
			{
				if(MK_CONTROL&flags)
				{
					
				}
				else
				{
					
				}
			}
		}
	}

	//!鼠标移动
	void GLineStringEditObject::MouseMove(GVector2dd& point,u32 flags)
	{
		if(MK_LBUTTON&flags)
		{
			if(EditHandleType==GMapEditObject::EHT_NODE_HANDLE)
			{
				if(MK_CONTROL&flags)
				{
					
				}
				else
				{
					LineString->SetCoordinate(HandleCookie,point);
					MapUIContext->DrawCache();
					Draw(EditGraphics);
				}
			}		
			else if(EditHandleType==GMapEditObject::EHT_EDGE_HANDLE)
			{
				GCoordinate coord1 = LineString->GetCoordinateAt(HandleCookie);
				GCoordinate coord2 = LineString->GetCoordinateAt(HandleCookie+1);

				GLine2dd line(coord1,coord2);
				GCoordinate pc=line.GetClosestPoint(point);


				MapUIContext->DrawCache();
				Draw(EditGraphics);

				Pen pen(Color::Red);

				PointF point;
				MapUIContext->GetDrawing()->MapToView(pc,point);

				EditGraphics->DrawRectangle(&pen,RectF(point.X-3,point.Y-3,6,6));

			}
		}	
	}

	//!鼠标抬起
	void GLineStringEditObject::MouseUp(GVector2dd& point,u32 flags)
	{
		if(MK_LBUTTON&flags)
		{
			if(EditHandleType==GMapEditObject::EHT_NODE_HANDLE)
			{
				if(MK_CONTROL&flags)
				{

				}
			}
			else if(EditHandleType==GMapEditObject::EHT_EDGE_HANDLE)
			{
				GCoordinate coord1 = LineString->GetCoordinateAt(HandleCookie);
				GCoordinate coord2 = LineString->GetCoordinateAt(HandleCookie+1);

				GLine2dd line(coord1,coord2);
				GCoordinate pc=line.GetClosestPoint(point);

				LineString->InsertCoordinate(HandleCookie+1,pc);
			}
		}	
	}

	//!结束编辑
	void GLineStringEditObject::EndEdit()
	{	
		EditHandleType=GMapEditObject::EHT_NONE;
		EditGraphics = NULL;
	}

	//!绘制自身
	void GLineStringEditObject::Draw(Graphics* graphics){
		int count = LineString->GetCoordinateCount();
		GCoordinate* coordinates=new GCoordinate[count];

		LineString->GetCoordinates(coordinates);
		PointF* points=new PointF[count];
		MapUIContext->GetDrawing()->MapToView(coordinates,points,count,&count);
		
		if(EditHandleType!=GMapEditObject::EHT_NONE)
		{
			Pen pen0(Color::Gray);
			graphics->DrawLines(&pen0,points,count);
		}

		Pen pen1(Color::Black,1.0);
		for(int i=0;i<count;i++)
		{
			graphics->DrawRectangle(&pen1,RectF(points[i].X-3,points[i].Y-3,6,6));
		}

		delete[] coordinates;
		delete[] points;	
	}

	//!修改之后
	void GLineStringEditObject::OnEdited(){}

}
