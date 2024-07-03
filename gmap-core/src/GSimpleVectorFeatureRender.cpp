#include "GSimpleVectorFeatureRender.h"
#include "GVectorDataCommand.h"

namespace gmap{
	GSimpleVectorFeatureRender::GSimpleVectorFeatureRender(GMarkerStyle* markStyle,GLineStyle*   lineStyle,GFillStyle*	  fillStyle)
	{
		MarkerStyle = markStyle;
		if(MarkerStyle)MarkerStyle->AddRef();

		FillStyle = fillStyle;
		if(FillStyle)FillStyle->AddRef();

		LineStyle = lineStyle;
		if(LineStyle)LineStyle->AddRef();
	}

	GSimpleVectorFeatureRender::~GSimpleVectorFeatureRender(void)
	{	
		if(MarkerStyle)MarkerStyle->Release();

		if(FillStyle)FillStyle->Release();

		if(LineStyle)LineStyle->Release();
	}

	void 	GSimpleVectorFeatureRender::InitQuery(GVectorDataQuery* pQuery)
	{

	}
	void GSimpleVectorFeatureRender::begin()
	{
		if(MarkerStyle)
			MarkerStyle->ResetDefault();
		if(LineStyle)
			LineStyle->ResetDefault();
		if(FillStyle)
			FillStyle->ResetDefault();
	}
	void 	GSimpleVectorFeatureRender::DrawFeature(GMapDrawing*  pDrawing,GVectorDataQueryResult* pCurrent)
	{
		DrawGeometry(pDrawing,pCurrent->GetGeometry());
	}

	void GSimpleVectorFeatureRender::DrawGeometry(GMapDrawing*  pDrawing,GGeometry* pGeometry)
	{
		GGeometry::EnumGeometryType gtype =	pGeometry->GetGeometryType();

		if(gtype==GGeometry::GT_POINT)
		{
			GStyleMapDrawing::DrawPoint(pDrawing,(GPoint*)pGeometry,MarkerStyle);
		}
		else if(gtype==GGeometry::GT_LINE_STRING)
		{
			GStyleMapDrawing::DrawLineString(pDrawing,(GLineString*)pGeometry,LineStyle);
		}
		else if(gtype==GGeometry::GT_POLYGON)
		{
			GStyleMapDrawing::DrawPolygon(pDrawing,(GPolygon*)pGeometry,LineStyle,FillStyle);
		}
		else if((gtype&GGeometry::GT_COLLECTION)!=NULL)
		{
			GMultiGeometry* pGeometries = (GMultiGeometry*)pGeometry;
			for(int i=0;i<pGeometries->GetGeometryCount();i++)
			{
				DrawGeometry(pDrawing,pGeometries->GetGeometryAt(i));
			}
		}
	}

	GMarkerStyle* GSimpleVectorFeatureRender::GetMarkerStyle()
	{
		return MarkerStyle;
	}

	void GSimpleVectorFeatureRender::SetMarkerStyle(GMarkerStyle* pStyle)
	{
		if(MarkerStyle)
		{
			MarkerStyle->Release();
			MarkerStyle=NULL;
		}
		MarkerStyle = pStyle;
		if(MarkerStyle)
		{
			MarkerStyle->AddRef();
		}
	}

	GLineStyle* GSimpleVectorFeatureRender::GetLineStyle()
	{
		return LineStyle;
	}

	void GSimpleVectorFeatureRender::SetLineStyle(GLineStyle* pStyle)
	{
		if(LineStyle)
		{
			LineStyle->Release();
			LineStyle=NULL;
		}
		LineStyle = pStyle;
		if(LineStyle)
		{
			LineStyle->AddRef();
		}
	}

	GFillStyle* GSimpleVectorFeatureRender::GetFillStyle()
	{
		return FillStyle;
	}


	void GSimpleVectorFeatureRender::SetFillStyle(GFillStyle* pStyle)
	{
		if(FillStyle)
		{
			FillStyle->Release();
			FillStyle=NULL;
		}
		FillStyle = pStyle;
		if(FillStyle)
		{
			FillStyle->AddRef();
		}
	}
}
