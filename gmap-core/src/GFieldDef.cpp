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

	//!��ȡ����
	const wstring& GFieldDef::GetName()
	{
		return Name;
	}

	//!��ȡ�ֶ�����
	GFieldDef::EnumFieldType  GFieldDef::GetFieldType()
	{
		return FieldType;
	}

	//!�õ��ֶγߴ�
	u32			   GFieldDef::GetFieldSize()
	{
		return FieldSize;
	}
}
