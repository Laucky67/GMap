#include "GLineStringElement.h"
#include "GGraphicsLayer.h"
#include "GMap.h"
#include "GLineStringEditObject.h"
#include "GHitTestHelper.h"

namespace gmap
{
	GLineStringElement::GLineStringElement()
	{
		LineString = NULL;
		LineStyle = NULL;
	}
	GLineStringElement::GLineStringElement(GLineString* lineString,GLineStyle* style)
	{
		LineString = lineString;
		LineString->AddRef();
		LineStyle = NULL;
		SetLineStyle(style);
	}


	GLineStringElement::~GLineStringElement(void)
	{
		LineString->Release();
		if(LineStyle)LineStyle->Release();
	}

	//!�õ�Ԫ������
	GGraphicsElement::EnumGraphicsElmementType GLineStringElement::GetElementType()
	{
		return GGraphicsElement::GE_LINE;
	}

	//!����
	void GLineStringElement::DrawElement(GMapDrawing* pDrawing)
	{
		int count = LineString->GetCoordinateCount();
		GCoordinate* coordinates=new GCoordinate[count];
		
		LineString->GetCoordinates(coordinates);
		PointF* points=new PointF[count];
		pDrawing->MapToView(coordinates,points,count,&count);

		if(LineStyle)
		{
			LineStyle->Draw(pDrawing->GetGraphics(),points,count,false);
		}
		else
 		{
			if(Layer!=NULL&&Layer->GetDefaultLineStyle()!=NULL)
			{
				Layer->GetDefaultLineStyle()->Draw(pDrawing->GetGraphics(),points,count,false);
			}
		}

		delete[] coordinates;
		delete[] points;
	}

	//!�õ���Χ��
	GRectangle2dd GLineStringElement::GetBoundingBox()
	{
		return LineString->GetBoundingBox();
	}

	//!���
	GLineStyle* GLineStringElement::GetLineStyle()
	{
		return LineStyle;
	}

	//!���÷��
	void	GLineStringElement::SetLineStyle(GLineStyle* style)
	{
		if(LineStyle)LineStyle->Release();
		LineStyle=style;
		if(LineStyle)LineStyle->AddRef();
	}

	//!�õ���
	GLineString*	GLineStringElement::GetLineString()
	{
		return LineString;
	}
			
	void GLineStringElement::Save(IWriteFile* pStream)
	{
		LineString->Save(pStream);
		u32 styleId = LineStyle->GetId();
		pStream->Write(styleId);
	}


	void GLineStringElement::Load(IReadFile* pStream)
	{
		LineString = GLineString::Create();
		LineString->Load(pStream);
		u32 styleId;
		pStream->Read(styleId);
		GStyle* pStyle = Layer->GetMap()->GetStyleGroup()->GetStyleById(styleId);
		
		SetLineStyle((GLineStyle*)pStyle);
	}

	f64	GLineStringElement::HitTest(const GCoordinate& point)
	{
		f64 e = 1e10;
		u32 index;
		f64 d1 = GHitTestHelper::GetNearestEdge(LineString,point,&index,false,e);
		f64 d2 = GHitTestHelper::GetNearestPoint(LineString,point,&index,e);

		return min(d1,d2);	
	}

	GMapEditObject* GLineStringElement::CreateEditObject()
	{
		GLineStringEditObject* pEditObject = new GLineStringEditObject(wstring(L"Line_Edit_Object"),LineString);
		return pEditObject;
	}

}