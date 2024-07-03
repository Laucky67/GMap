#include "GFieldDef.h"
namespace gmap{

	//!
	GFieldDef::GFieldDef(const wstring& name,EnumFieldType fldType,u32 fieldSize)
		:Name(name),FieldType(fldType),FieldSize(fieldSize)
	{

	}

	GFieldDef::~GFieldDef(void)
	{
	}

	//!获取名字
	const wstring& GFieldDef::GetName()
	{
		return Name;
	}

	//!获取字段类型
	GFieldDef::EnumFieldType  GFieldDef::GetFieldType()
	{
		return FieldType;
	}

	//!得到字段尺寸
	u32			   GFieldDef::GetFieldSize()
	{
		return FieldSize;
	}
}
