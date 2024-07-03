#include "GVectorDataLayerDef.h"
namespace gmap{

	GVectorDataLayerDef::GVectorDataLayerDef(GVectorDataLayer::EnumVectorLayerType layerType)
	{
		LayerType = layerType;
	}

		GVectorDataLayerDef::~GVectorDataLayerDef(void)
		{
			int count = FieldDefList.size();
			for(int i=0;i<count;i++)
			{
				FieldDefList[i]->Release();
			}
			FieldDefList.clear();
		}

		//!图层类型
		GVectorDataLayer::EnumVectorLayerType GVectorDataLayerDef::GetLayerType()
		{
			return LayerType;
		}


		//!得到字段的个数
		u32 GVectorDataLayerDef::GetFieldCount()
		{
			return FieldDefList.size();
		}

		//!得到字段的定义
		GFieldDef* GVectorDataLayerDef::GetFieldDef(u32 index)
		{
			return FieldDefList[index];
		}

		//!添加字段
		void GVectorDataLayerDef::AddField(const wstring& name,GFieldDef::EnumFieldType fieldType,u32 size)
		{
			GFieldDef* pDef = new GFieldDef(name,fieldType,size);
			FieldDefList.push_back(pDef);
		}


}