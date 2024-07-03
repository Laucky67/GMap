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

	//!����
	void GMapLayer::Draw(GMapDrawing* pDrawing)
	{
		
	}

	//!�õ���Χ��
	GRectangle2dd GMapLayer::GetBoundingBox()
	{
		return DefaultBoundingBox;
	}

	//!����Ĭ�ϰ�Χ��
	void GMapLayer::SetDefaultBoundingBox(GRectangle2dd& bbox)
	{
		DefaultBoundingBox = bbox;
	}

	//!��ȡ����
	const wstring& GMapLayer::GetName()
	{
		return Name;
	}

	//!��������
	void GMapLayer::SetName(const wstring& name)
	{
		Name = name;
	}

	GMapLayer::EnumLayerType GMapLayer::GetLayerType()
	{
		return GMapLayer::LT_UNKNOWN;
	}

	//!�ɼ���
	bool GMapLayer::GetVisible()
	{
		return Visible;
	}

	//!���ÿɼ���
	void GMapLayer::SetVisible(bool visible)
	{
		Visible = visible;
	}
	
	void GMapLayer::AttachMap(GMap* map)
	{
		Map = map;
	}
	
}
