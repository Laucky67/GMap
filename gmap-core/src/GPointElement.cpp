#include "GPointElement.h"
#include "GGraphicsLayer.h"
#include "GMap.h"
#include "GHitTestHelper.h"
#include "GPointEditObject.h"

namespace gmap
{
	
	GPointElement::GPointElement()
	{
		Point=NULL;
		MarkerStyle = NULL;
	}

	GPointElement::GPointElement(GPoint* point,GMarkerStyle* style)
	{
		Point = point;
		Point->AddRef();
		MarkerStyle = NULL;
		SetMarkerStyle(style);
	}

	GPointElement::GPointElement(f64 x,f64 y,GMarkerStyle* style)
	{
		Point = GPoint::Create(x,y);
		MarkerStyle = NULL;
		SetMarkerStyle(style);
	}

	GPointElement::~GPointElement(void)
	{
		Point->Release();
		if(MarkerStyle)MarkerStyle->Release();
	}

	//!�õ�Ԫ������
	GGraphicsElement::EnumGraphicsElmementType GPointElement::GetElementType()
	{
		return GGraphicsElement::GE_POINT;
	}

	//!����
	void GPointElement::DrawElement(GMapDrawing* pDrawing)
	{
		GCoordinate coordinate;
		Point->GetCoordinate(&coordinate);
		PointF point;
		pDrawing->MapToView(coordinate,point);

		if(MarkerStyle)
		{
			MarkerStyle->Draw(pDrawing->GetGraphics(),point);
		}
		else
 		{
			if(Layer!=NULL&&Layer->GetDefaultMarkerStyle()!=NULL)
			{
				Layer->GetDefaultMarkerStyle()->Draw(pDrawing->GetGraphics(),point);
			}
		}
	}

	//!�õ���Χ��
	GRectangle2dd GPointElement::GetBoundingBox()
	{
		return Point->GetBoundingBox();
	}

	//!���
	GMarkerStyle* GPointElement::GetMarkerStyle()
	{
		return MarkerStyle;
	}

	//!���÷��
	void	GPointElement::SetMarkerStyle(GMarkerStyle* style)
	{
		if(MarkerStyle)MarkerStyle->Release();
		MarkerStyle=style;
		if(MarkerStyle)MarkerStyle->AddRef();
	}

	//!�õ���
	GPoint*	GPointElement::GetPoint()
	{
		return Point;
	}

	void GPointElement::Save(IWriteFile* pStream)
	{
		this->Point->Save(pStream);
		u32 styleId = MarkerStyle->GetId();
		pStream->Write(styleId);
	}

	void GPointElement::Load(IReadFile* pStream)
	{
		Point = GPoint::Create();
		Point->Load(pStream);
		u32 styleId;
		pStream->Read(styleId);
		GStyle* pStyle = Layer->GetMap()->GetStyleGroup()->GetStyleById(styleId);
		SetMarkerStyle((GMarkerStyle*)pStyle);		
	}

	
	f64	GPointElement::HitTest(const GCoordinate& point)
	{
		return GHitTestHelper::GetDistance(Point,point);
	}

	GMapEditObject* GPointElement::CreateEditObject()
	{
		GPointEditObject* pEditObject = new GPointEditObject(wstring(L"Point_Edit_Object"),Point);
		return pEditObject;
	}
}