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

	//!与上下文进行关联
	void GMapEditObject::AttachUIContext(IMapUIContext* pUIContext)
	{
		MapUIContext = pUIContext;
	}

	//!编辑对象的名字
	const wstring& GMapEditObject::GetName()
	{
		return Name;
	}

}