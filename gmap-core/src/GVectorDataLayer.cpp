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


	//!�õ�����
	const wstring& GVectorDataLayer::GetName()
	{
		return Name;
	}


	//!�õ��ֶεĸ���
	u32 GVectorDataLayer::GetFieldCount()
	{
		return FieldDefList.size();
	}

	//!�õ��ֶεĶ���
	GFieldDef* GVectorDataLayer::GetFieldDef(u32 index)
	{
		return FieldDefList[index];
	}
	
	//!�õ��ֶζ���
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


	//!�������ϵ
	GCoordinateSystem*	GVectorDataLayer::GetCoordinateSystem()
	{
		return CoordinateSystem;
	}

	//!��������ϵ
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
