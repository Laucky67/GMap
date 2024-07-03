#include "GPolygonEditObject.h"
#include "GHitTestHelper.h"
#include "GMapDrawing.h"

namespace gmap{

	GPolygonEditObject::GPolygonEditObject(const wstring& name,GPolygon* polygon)
		:GMapEditObject(name)
	{
		Polygon = polygon;
		Polygon->AddRef();
	}


	GPolygonEditObject::~GPolygonEditObject(void)
	{
		Polygon->Release();
	}

	//!执行测试
	bool GPolygonEditObject::HitTest(GVector2dd& hitPoint,f64 tolerence)
	{
		u32 index;
		f64 d1=GHitTestHelper::GetNearestPoint(Polygon,hitPoint,&index,tolerence);

		if(d1<tolerence)
		{
			return true;
		}
		else
		{
			f64 d2=GHitTestHelper::GetNearestEdge(Polygon,hitPoint,&index,tolerence);
		}
		return false;
	}

	//!查询当前位置上支持编辑类型
	GMapEditObject::EnumEditHandleType GPolygonEditObject::QueryEditHandle(GVector2dd& hitPoint,u32* pHandleCookie,f64 tolerence)
	{
		u32 index;
		f64 d1=GHitTestHelper::GetNearestPoint(Polygon,hitPoint,&index,tolerence);
		if(d1<tolerence)
		{
			*pHandleCookie = index;
			return GMapEditObject::EHT_NODE_HANDLE;
		}
		else
		{
			d1=GHitTestHelper::GetNearestEdge(Polygon,hitPoint,&index,tolerence);
			if(d1<tolerence)
			{
				*pHandleCookie = index;
				return GMapEditObject::EHT_EDGE_HANDLE; 
			}
		}
		
		return GMapEditObject::EHT_NONE;	
	}


	void GPolygonEditObject::DrawEditEffect(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		Pen pen(Color::Red,2.0);

		GLineString* LineString=Polygon->GetExternalRing();

		if(handlerType==GMapEditObject::EHT_NODE_HANDLE)
		{
			GCoordinate coord = LineString->GetCoordinateAt(handleCookie);
			PointF point;
			MapUIContext->GetDrawing()->MapToView(coord,point);

			graphics->DrawRectangle(&pen,RectF(point.X-4,point.Y-4,8,8));

		}
		else if(handlerType==GMapEditObject::EHT_EDGE_HANDLE)
		{		
			GCoordinate coord1 = LineString->GetCoordinateAt(handleCookie);
			u32 index2 = handleCookie+1;
			if(index2==LineString->GetCoordinateCount())
			{
				index2=0;
			}
			GCoordinate coord2 = LineString->GetCoordinateAt(index2);
			PointF point1,point2;
			MapUIContext->GetDrawing()->MapToView(coord1,point1);
			MapUIContext->GetDrawing()->MapToView(coord2,point2);

			graphics->DrawLine(&pen,point1,point2);
		}
	
	}

	//!开始编辑
	void GPolygonEditObject::BeginEdit(Graphics* graphics,EnumEditHandleType handlerType,GVector2dd& hitPoint,u32 handleCookie)
	{
		EditHandleType = handlerType;
		HandleCookie =	 handleCookie;
		EditGraphics=	 graphics;
	}

	//!鼠标落下
	void GPolygonEditObject::MouseDown(GVector2dd& point,u32 flags){
		
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
	void GPolygonEditObject::MouseMove(GVector2dd& point,u32 flags)
	{
		
		GLineString* LineString = Polygon->GetExternalRing();


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
		}	
	}

	//!鼠标抬起
	void GPolygonEditObject::MouseUp(GVector2dd& point,u32 flags)
	{
		GLineString* LineString = Polygon->GetExternalRing();
		if(MK_LBUTTON&flags)
		{
			if(EditHandleType==GMapEditObject::EHT_NODE_HANDLE)
			{
				if(MK_CONTROL&flags)
				{

				}
			}
		}	
	}

	//!结束编辑
	void GPolygonEditObject::EndEdit()
	{	
		EditHandleType=GMapEditObject::EHT_NONE;
		EditGraphics = NULL;
	}

	//!绘制自身
	void GPolygonEditObject::Draw(Graphics* graphics){
		GLineString* LineString = Polygon->GetExternalRing();

		int count = LineString->GetCoordinateCount();
		GCoordinate* coordinates=new GCoordinate[count];

		LineString->GetCoordinates(coordinates);
		PointF* points=new PointF[count];
		MapUIContext->GetDrawing()->MapToView(coordinates,points,count,&count);
			
		if(EditHandleType!=GMapEditObject::EHT_NONE)
		{
			SolidBrush brush(Color(60,0,0,0));
			graphics->FillPolygon(&brush,points,count);

			Pen pen0(Color::Gray);
			graphics->DrawPolygon(&pen0,points,count);
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
	void GPolygonEditObject::OnEdited(){}

}
