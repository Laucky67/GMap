#include "GVectorDataLayer.h"
#include "GFieldDef.h"

namespace gmap{

	GVectorDataLayer::GVectorDataLayer(const wstring& name)
	{
		Name  = name;
		CoordinateSystem = NULL;
	}

	GVectorDataLayer::~GVectorDataLayer(void)
	{
		u32 count =FieldDefList.size();
		for(int i=0;i<count;i++)
		{
			FieldDefList.at(i)->Release();
		}
		if(CoordinateSystem)
		{
			CoordinateSystem->Release();
		}
	}


	//!得到名字
	const wstring& GVectorDataLayer::GetName()
	{
		return Name;
	}


	//!得到字段的个数
	u32 GVectorDataLayer::GetFieldCount()
	{
		return FieldDefList.size();
	}

	//!得到字段的定义
	GFieldDef* GVectorDataLayer::GetFieldDef(u32 index)
	{
		return FieldDefList[index];
	}
	
	//!得到字段定义
	GFieldDef*  GVectorDataLayer::GetFieldDef(const wstring& name)
	{
		u32 count = FieldDefList.size();
		for(int i=0;i<count;i++)
		{
			if(name == FieldDefList.at(i)->GetName())
			{
				return FieldDefList.at(i);
			}
		}
		return NULL;
	}


	//!获得坐标系
	GCoordinateSystem*	GVectorDataLayer::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}

	//!设置坐标系
	void				GVectorDataLayer::SetCoordinateSystem(GCoordinateSystem* pCS)
	{
		if(CoordinateSystem)
		{
			CoordinateSystem->Release();
		}

		CoordinateSystem= pCS;

		if(CoordinateSystem)
		{
			CoordinateSystem->AddRef();
		}
	}
}
