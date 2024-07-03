#include "GGraphicsElement.h"
namespace gmap{

	GGraphicsElement::GGraphicsElement(void)
	{
		Id = -1;
		Layer = NULL;
	}

	GGraphicsElement::~GGraphicsElement(void)
	{
	
	}

	void GGraphicsElement::AttachLayer(u32 id,GGraphicsLayer* layer)
	{
		Id = id;
		Layer = layer;
	}

	//!�õ�Id
	u32 GGraphicsElement::GetId()
	{
		return Id;
	}

	//!�õ�ͼ��
	GGraphicsLayer* GGraphicsElement::GetLayer()
	{
		return Layer;
	}


}
