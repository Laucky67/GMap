#include "GMapLayer.h"
namespace gmap{

	GMapLayer::GMapLayer()
	{
		Map = NULL;
		Visible = true;
	}

	GMapLayer::~GMapLayer(void)
	{

	}

	GMap* GMapLayer::GetMap()
	{
		return Map;
	}

	//!绘制
	void GMapLayer::Draw(GMapDrawing* pDrawing)
	{
		
	}

	//!得到包围盒
	GRectangle2dd GMapLayer::GetBoundingBox()
	{
		return DefaultBoundingBox;
	}

	//!设置默认包围盒
	void GMapLayer::SetDefaultBoundingBox(GRectangle2dd& bbox)
	{
		DefaultBoundingBox = bbox;
	}

	//!获取名字
	const wstring& GMapLayer::GetName()
	{
		return Name;
	}

	//!设置名字
	void GMapLayer::SetName(const wstring& name)
	{
		Name = name;
	}

	GMapLayer::EnumLayerType GMapLayer::GetLayerType()
	{
		return GMapLayer::LT_UNKNOWN;
	}

	//!可见性
	bool GMapLayer::GetVisible()
	{
		return Visible;
	}

	//!设置可见性
	void GMapLayer::SetVisible(bool visible)
	{
		Visible = visible;
	}
	
	void GMapLayer::AttachMap(GMap* map)
	{
		Map = map;
	}
	
}
