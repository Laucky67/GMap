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

	//!��ͼ����
	const wstring&	GMap::GetName()
	{
		return Name;
	}

	//!��ͼ����
	void GMap::SetName(const wstring& name)
	{
		Name = name;
	}

	//!�õ�StyleGroup
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

	//!����
	void GMap::Draw(GMapDrawing* pDrawing)
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			Layers[iLayer]->Draw(pDrawing);
		}

	}

	//!�õ�ͼ��ĸ���
	u32	GMap::GetLayerCount()
	{
		return Layers.size();
	}

	//!�õ�ͼ��
	GMapLayer* GMap::GetLayer(u32 index)
	{
		return Layers[index];
	}

	//!�õ�ͼ��
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

	//!���ͼ��
	void GMap::AddLayer(GMapLayer* pLayer)
	{
		Layers.push_back(pLayer);
		pLayer->AddRef();
		pLayer->AttachMap(this);
		CurrentLayer = pLayer;
	}

	//!�Ƴ�ͼ��
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

	//!����ͼ��
	bool GMap::MoveLayer(u32 fromLayer, u32 toLayer)
	{
		if (fromLayer >= 0 && fromLayer < Layers.size()
			&& toLayer >= 0 && toLayer < Layers.size())
		{
			//TODO:ʵ��֮
			return true;
		}
		else
		{
			return false;
		}
	}

	//!�Ƴ�����ͼ��
	void GMap::ClearLayers()
	{
		int count = Layers.size();
		for (int iLayer = 0; iLayer < count; iLayer++)
		{
			Layers[iLayer]->Release();
		}
		Layers.clear();
	}

	//!�õ���Χ��
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


	//!���õ�ͼ����ϵ
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

	//!�õ�����ϵ
	GCoordinateSystem* GMap::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}
}