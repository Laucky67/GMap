#include "GSimpleAddElementTool.h"
namespace gmap {
	GAddPointElementTool::GAddPointElementTool(IMapUIContext * context) :
		GMapUITool(context)
	{

	}
	GAddPointElementTool::~GAddPointElementTool(void)
	{

	}
	bool GAddPointElementTool::OnMouseDown(u32 x, s32 y, s32 button)
	{
		if (button == MK_LBUTTON)
		{
			GMap* pMap = MapUIContext->GetMap();
			if (pMap != NULL
				&& pMap->GetCurrentLayer() != NULL
				&& pMap->GetCurrentLayer()->GetLayerType() == GMapLayer::LT_GRAPHICS)
			{
				f64 mapx, mapy;
				MapUIContext->GetDrawing()->ViewToMap(x, y, mapx, mapy);

				GGraphicsLayer* pLayer = (GGraphicsLayer*)pMap->GetCurrentLayer();

				GPointElement* pPointElement = new GPointElement(mapx, mapy, pLayer->GetDefaultMarkerStyle());
				pLayer->AppendElement(pPointElement);
				pPointElement->Release();

				MapUIContext->Redraw();
			}
		}
		return true;
	}
	void GAddPointElementTool::Draw(Graphics * graphics) {}


	GAddLineElementTool::GAddLineElementTool(IMapUIContext * context) :
		GMapUITool(context)
	{

	}
	GAddLineElementTool::~GAddLineElementTool(void)
	{

	}
	bool GAddLineElementTool::OnMouseDown(u32 x, s32 y, s32 button)
	{
		GMap* pMap = MapUIContext->GetMap();
		if (pMap == NULL
			|| pMap->GetCurrentLayer() == NULL
			|| pMap->GetCurrentLayer()->GetLayerType() != GMapLayer::LT_GRAPHICS)
		{
			return false;
		}

		if (button == MK_LBUTTON)
		{
			f64 mapx, mapy;
			MapUIContext->GetDrawing()->ViewToMap(x, y, mapx, mapy);
			MapUIContext->DrawCache(0,0);
			Points.push_back(GCoordinate(mapx, mapy));

		}
		else if (button == MK_RBUTTON)
		{
			GGraphicsLayer* pLayer = (GGraphicsLayer*)pMap->GetCurrentLayer();
			if (Points.size() > 1)
			{
				GLineString* linestring = GLineString::Create(Points);
				GLineStringElement* pLineElement = new GLineStringElement(linestring, pLayer->GetDefaultLineStyle());
				pLayer->AppendElement(pLineElement);
				pLineElement->Release();
				linestring->Release();
				Points.clear();
			}
			this->MapUIContext->Redraw();
		}
		return true;
	}
	void GAddLineElementTool::Draw(Graphics * graphics)
	{
		GMap* pMap = MapUIContext->GetMap();

		if (pMap == NULL
			|| pMap->GetCurrentLayer() == NULL
			|| pMap->GetCurrentLayer()->GetLayerType() != GMapLayer::LT_GRAPHICS)
		{
			return;
		}
		if (Points.size() > 1)
		{
			GGraphicsLayer* pLayer = (GGraphicsLayer*)pMap->GetCurrentLayer();

			GMapDrawing* pDrawing = MapUIContext->GetDrawing();
			PointF* GpPoints = new PointF[Points.size()];
			for (int i = 0; i < Points.size(); i++)
			{
				pDrawing->MapToView(Points[i], GpPoints[i]);
			}
			pLayer->GetDefaultLineStyle()->Draw(graphics, GpPoints, Points.size(), false);
			delete[] GpPoints;
		}
	}
	GAddPolygonElementTool::GAddPolygonElementTool(IMapUIContext * context) :
		GMapUITool(context)
	{

	}
	GAddPolygonElementTool::~GAddPolygonElementTool(void)
	{

	}
	bool GAddPolygonElementTool::OnMouseDown(u32 x, s32 y, s32 button)
	{
		GMap* pMap = MapUIContext->GetMap();
		if (pMap == NULL
			|| pMap->GetCurrentLayer() == NULL
			|| pMap->GetCurrentLayer()->GetLayerType() != GMapLayer::LT_GRAPHICS)
		{
			return false;
		}

		if (button == MK_LBUTTON)
		{
			f64 mapx, mapy;
			MapUIContext->GetDrawing()->ViewToMap(x, y, mapx, mapy);
			//MapUIContext->Redraw();
			Points.push_back(GCoordinate(mapx, mapy));
			MapUIContext->DrawCache(0, 0);

		}
		else if (button == MK_RBUTTON)
		{
			GGraphicsLayer* pLayer = (GGraphicsLayer*)pMap->GetCurrentLayer();
			if (Points.size() > 2)
			{
				GPolygon* polygon = GPolygon::Create(Points);
				GPolygonElement* pPolygonElement = new GPolygonElement(
												polygon, 
												pLayer->GetDefaultLineStyle(),
												pLayer->GetDefaultFillStyle());
				pLayer->AppendElement(pPolygonElement);
				pPolygonElement->Release();
				polygon->Release();
				Points.clear();
			}
			this->MapUIContext->Redraw();
		}
		return true;
	}
	void GAddPolygonElementTool::Draw(Graphics * graphics)
	{
		GMap* pMap = MapUIContext->GetMap();

		if (pMap == NULL
			|| pMap->GetCurrentLayer() == NULL
			|| pMap->GetCurrentLayer()->GetLayerType() != GMapLayer::LT_GRAPHICS)
		{
			return;
		}
		if (Points.size() > 0)
		{
			GGraphicsLayer* pLayer = (GGraphicsLayer*)pMap->GetCurrentLayer();


			GMapDrawing* pDrawing = MapUIContext->GetDrawing();
			PointF* GpPoints = new PointF[Points.size()];
			for (int i = 0; i < Points.size(); i++)
			{
				pDrawing->MapToView(Points[i], GpPoints[i]);
			}
			pLayer->GetDefaultFillStyle()->Draw(graphics, GpPoints, Points.size());
			pLayer->GetDefaultLineStyle()->Draw(graphics, GpPoints, Points.size(), true);
			delete[] GpPoints;
		}
	}
}
