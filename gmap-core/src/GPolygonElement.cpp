#include "GPolygonElement.h"
#include "GGraphicsLayer.h"
#include "GMap.h"
#include "GHitTestHelper.h"
#include "GPolygonEditObject.h"

namespace gmap
{
	GPolygonElement::GPolygonElement()
	{
		Polygon=NULL;
		LineStyle = NULL;
		FillStyle = NULL;
	}

	GPolygonElement::GPolygonElement(GPolygon* polygon,GLineStyle* lineStyle,GFillStyle* fillStyle)
	{
		Polygon = polygon;
		Polygon->AddRef();
		LineStyle = NULL;
		SetLineStyle(lineStyle);
		FillStyle = NULL;
		SetFillStyle(fillStyle);

	}


	GPolygonElement::~GPolygonElement(void)
	{
		Polygon->Release();
		if(LineStyle)LineStyle->Release();
		if(FillStyle)FillStyle->Release();
	}

	//!�õ�Ԫ������
	GGraphicsElement::EnumGraphicsElmementType GPolygonElement::GetElementType()
	{
		return GGraphicsElement::GE_POLYGON;
	}

	//!����
	void GPolygonElement::DrawElement(GMapDrawing* pDrawing)
	{
		GLinearRing* externalRing = Polygon->GetExternalRing();
		vector<GraphicsPath*> paths;
		GraphicsPath path0;
		{
			int count = externalRing->GetCoordinateCount();
			GCoordinate* coordinates=new GCoordinate[count];

			externalRing->GetCoordinates(coordinates);
			PointF* points=new PointF[count];
			pDrawing->MapToView(coordinates,points,count,&count);

			path0.AddPolygon(points,count);
			delete[] coordinates;
			delete[] points;
			paths.push_back(path0.Clone());
		}
		Region region(&path0);
		for(int i=0;i<Polygon->GetInternalRingCount();i++)
		{
			GLinearRing* ring = Polygon->GetInternalRingAt(i);
			int count = ring->GetCoordinateCount();
			GCoordinate* coordinates=new GCoordinate[count];

			ring->GetCoordinates(coordinates);
			PointF* points=new PointF[count];
			pDrawing->MapToView(coordinates,points,count,&count);
			GraphicsPath path1;
			path1.AddPolygon(points,count);

			paths.push_back(path1.Clone());


			region.Exclude(&path1);
			delete[] coordinates;
			delete[] points;
		}

		if(FillStyle)
		{
			FillStyle->Draw(pDrawing->GetGraphics(),&region);
		}
		else
		{
			if(Layer!=NULL&&Layer->GetDefaultFillStyle()!=NULL)
			{
				Layer->GetDefaultFillStyle()->Draw(pDrawing->GetGraphics(),&region);
			}
		}

		for(int i=0;i<paths.size();i++)
		{
			if(LineStyle)
			{
				LineStyle->Draw(pDrawing->GetGraphics(),paths[i]);
			}
			else
			{
				if(Layer!=NULL&&Layer->GetDefaultLineStyle()!=NULL)
				{
					Layer->GetDefaultLineStyle()->Draw(pDrawing->GetGraphics(),paths[i]);
				}
			}
			delete paths[i];
		}
	}

	//!�õ���Χ��
	GRectangle2dd GPolygonElement::GetBoundingBox()
	{
		return Polygon->GetBoundingBox();
	}

	//!���
	GLineStyle* GPolygonElement::GetLineStyle()
	{
		return LineStyle;
	}

	//!���÷��
	void	GPolygonElement::SetLineStyle(GLineStyle* style)
	{
		if(LineStyle)LineStyle->Release();
		LineStyle=style;
		if(LineStyle)LineStyle->AddRef();
	}

	//!���
	GFillStyle* GPolygonElement::GetFillStyle()
	{
		return FillStyle;
	}

	//!���÷��
	void	GPolygonElement::SetFillStyle(GFillStyle* style)
	{
		if(FillStyle)FillStyle->Release();
		FillStyle=style;
		if(FillStyle)FillStyle->AddRef();
	}

	//!�õ���
	GPolygon*	GPolygonElement::GetPolygon()
	{
		return Polygon;
	}

	
	void  GPolygonElement::Save(IWriteFile* pStream)
	{
		Polygon->Save(pStream);
		u32 styleId = LineStyle->GetId();
		pStream->Write(styleId);
		styleId = FillStyle->GetId();
		pStream->Write(styleId);
	}

	void GPolygonElement::Load(IReadFile* pStream)
	{
		Polygon = GPolygon::Create();
		Polygon->Load(pStream);
		u32 styleId;
		GStyle* pStyle;
		pStream->Read(styleId);
		pStyle = Layer->GetMap()->GetStyleGroup()->GetStyleById(styleId);
		SetLineStyle((GLineStyle*)pStyle);

		pStream->Read(styleId);
		pStyle = Layer->GetMap()->GetStyleGroup()->GetStyleById(styleId);
		SetFillStyle((GFillStyle*)pStyle);
	}

	f64	GPolygonElement::HitTest(const GCoordinate& point)
	{
		
		if(GHitTestHelper::PointInPolygon(Polygon,point))
			return 0;

		f64 e = 1e10;
		u32 index;

		f64 d1 = GHitTestHelper::GetNearestEdge(Polygon,point,&index,e);
		f64 d2 = GHitTestHelper::GetNearestPoint(Polygon,point,&index,e);

		return min(d1,d2);	
	}

	GMapEditObject* GPolygonElement::CreateEditObject()
	{
		GPolygonEditObject* pEditObject = new GPolygonEditObject(wstring(L"Line_Edit_Object"),Polygon);
		return pEditObject;
	}
}