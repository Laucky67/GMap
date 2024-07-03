#include "GStyle.h"
namespace gmap{

	GStyle::GStyle(GStyleGroup* group,u32 id,const wstring& name)
	{
		Group = group;
		Id = id;
		Name = name;
	}

	GStyle::~GStyle(void)
	{
	
	}

	GStyleGroup* GStyle::GetGroup()
	{
		return Group;
	}

	u32	GStyle::GetId()
	{
		return Id;
	}

	//!����
	const wstring& GStyle::GetName()
	{
		return Name;
	}

	//!����
	void GStyle::SetName(const wstring& name)
	{
		Name =name;
	}

	GStyle::EnumStyleType GStyle::GetStyleType()
	{
		return GStyle::ST_UNKNOWN_STYLE;
	}
}
