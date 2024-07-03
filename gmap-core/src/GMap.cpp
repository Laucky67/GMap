#include "GMap.h"
#include "GMapLayer.h"
#include "GGraphicsLayer.h"
namespace gmap
{
	GMap::GMap(const wstring& name, GStyleGroup* styleGroup,  GCoordinateSystem* pCS)
	{
		Name = name;
		StyleGroup = styleGroup;
		StyleGroup->AddRef();
		CurrentLayer = NULL;

		CoordinateSystem = pCS;
		if (CoordinateSystem)
		{
			CoordinateSystem->AddRef();
		}
	}

	GMap::~GMap(void)
	{
		ClearLayers();
		StyleGroup->Release();

		if (CoordinateSystem)
		{
			CoordinateSystem->Release();
		}
	}

	//!地图名称
	const wstring&	GMap::GetName()
	{
		return Name;
	}

	//!地图名称
	void GMap::SetName(const wstring& name)
	{
		Name = name;
	}

	//!得到StyleGroup
	GStyleGroup*	GMap::GetStyleGroup()
	{
		return StyleGroup;
	}

	GMapLayer* GMap::GetCurrentLayer()
	{
		return CurrentLayer;
	}

	void GMap::SetCurrentLayer(GMapLayer* layer)
	{
		CurrentLayer = layer;
	}

	GGraphicsLayer*	GMap::GetDefaultGraphicsLayer()
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			if (Layers[iLayer]->GetLayerType() == GMapLayer::LT_GRAPHICS)
			{
				return (GGraphicsLayer*)Layers[iLayer];
			}
		}


		GGraphicsLayer* pNewLayer = new GGraphicsLayer(wstring(L"Default Graphics"));
		AddLayer(pNewLayer);

		GStyleGroup* pStyleGroup = GetStyleGroup();

		for (int i = 0; i < pStyleGroup->GetStyleCount(); i++)
		{
			GStyle* pStyle = pStyleGroup->GetStyleAt(i);
			if (pStyle->GetStyleType() == GStyle::ST_MARKER_STYLE)
			{
				pNewLayer->SetDefaultMarkerStyle((GMarkerStyle*)pStyle);
			}
			else if (pStyle->GetStyleType() == GStyle::ST_LINE_STYLE)
			{

				pNewLayer->SetDefaultLineStyle((GLineStyle*)pStyle);
			}
			else if (pStyle->GetStyleType() == GStyle::ST_FILL_STYLE)
			{
				pNewLayer->SetDefaultFillStyle((GFillStyle*)pStyle);
			}
		}

		pNewLayer->Release();
		return pNewLayer;
	}

	//!绘制
	void GMap::Draw(GMapDrawing* pDrawing)
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			Layers[iLayer]->Draw(pDrawing);
		}

	}

	//!得到图层的个数
	u32	GMap::GetLayerCount()
	{
		return Layers.size();
	}

	//!得到图层
	GMapLayer* GMap::GetLayer(u32 index)
	{
		return Layers[index];
	}

	//!得到图层
	GMapLayer* GMap::GetLayer(const wstring& name)
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			if (Layers[iLayer]->GetName() == name)
			{
				return Layers.at(iLayer);
			}

		}
		return NULL;
	}

	//!添加图层
	void GMap::AddLayer(GMapLayer* pLayer)
	{
		Layers.push_back(pLayer);
		pLayer->AddRef();
		pLayer->AttachMap(this);
		CurrentLayer = pLayer;
	}

	//!移除图层
	void GMap::RemoveLayer(u32 iLayer)
	{
		if (iLayer >= 0 && iLayer < Layers.size())
		{
			if (Layers[iLayer] == CurrentLayer)
			{
				CurrentLayer = NULL;
			}
			Layers[iLayer]->Release();
			Layers.erase(Layers.begin() + iLayer);
		}
	}

	//!上移图层
	bool GMap::MoveLayer(u32 fromLayer, u32 toLayer)
	{
		if (fromLayer >= 0 && fromLayer < Layers.size()
			&& toLayer >= 0 && toLayer < Layers.size())
		{
			//TODO:实现之
			return true;
		}
		else
		{
			return false;
		}
	}

	//!移除所有图层
	void GMap::ClearLayers()
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			Layers[iLayer]->Release();
		}
		Layers.clear();
	}

	//!得到包围盒
	GRectangle2dd GMap::GetMapExtent()
	{
		GRectangle2dd box0;
		int count = Layers.size();
		if (count > 0)
		{
			box0 = Layers[0]->GetBoundingBox();
			for (int iLayer = 1; iLayer < count; iLayer++)
			{
				GRectangle2dd box1 = Layers[iLayer]->GetBoundingBox();
				box0.AddRectangle(box1);
			}
		}
		else
		{
			box0 = GRectangle2dd(-1000, -1000, 1000, 1000);
		}
		return box0;
	}


	//!设置地图坐标系
	void GMap::SetCoordinateSystem(GCoordinateSystem* pCS)
	{
		if (CoordinateSystem)
		{
			CoordinateSystem->Release();
		}
		CoordinateSystem = pCS;
		if (CoordinateSystem)
		{
			CoordinateSystem->AddRef();
		}
		for (auto it = Layers.begin(), end = Layers.end(); it != end; it++)
		{
			(*it)->UpdateCache();
		}
	}

	//!得到坐标系
	GCoordinateSystem* GMap::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}
}