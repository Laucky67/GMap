#include "GMapEditObject.h"

namespace gmap{

	GMapEditObject::GMapEditObject(const wstring& name)
	{
		Name = name;
		MapUIContext = NULL;
	}

	GMapEditObject::~GMapEditObject(void)
	{
	
	}

	//!�������Ľ��й���
	void GMapEditObject::AttachUIContext(IMapUIContext* pUIContext)
	{
		MapUIContext = pUIContext;
	}

	//!�༭���������
	const wstring& GMapEditObject::GetName()
	{
		return Name;
	}

}