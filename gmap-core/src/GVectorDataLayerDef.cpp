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

		//!ͼ������
		GVectorDataLayer::EnumVectorLayerType GVectorDataLayerDef::GetLayerType()
		{
			return LayerType;
		}


		//!�õ��ֶεĸ���
		u32 GVectorDataLayerDef::GetFieldCount()
		{
			return FieldDefList.size();
		}

		//!�õ��ֶεĶ���
		GFieldDef* GVectorDataLayerDef::GetFieldDef(u32 index)
		{
			return FieldDefList[index];
		}

		//!����ֶ�
		void GVectorDataLayerDef::AddField(const wstring& name,GFieldDef::EnumFieldType fieldType,u32 size)
		{
			GFieldDef* pDef = new GFieldDef(name,fieldType,size);
			FieldDefList.push_back(pDef);
		}


}