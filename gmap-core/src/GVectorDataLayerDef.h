#ifndef __G_VECTOR_DATA_LAYER_DEF_H__
#define __G_VECTOR_DATA_LAYER_DEF_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GFieldDef.h"
#include "GVectorDataLayer.h"

namespace gmap{

	//!ͼ�㶨��
	class GMAPCORE_API GVectorDataLayerDef:public GUnknown
	{
	public:
		GVectorDataLayerDef(GVectorDataLayer::EnumVectorLayerType layerType);

		~GVectorDataLayerDef(void);

		//!ͼ������
		GVectorDataLayer::EnumVectorLayerType GetLayerType();

		//!�õ��ֶεĸ���
		u32 GetFieldCount();

		//!�õ��ֶεĶ���
		GFieldDef* GetFieldDef(u32 index);

		//!����ֶ�
		void AddField(const wstring& name,GFieldDef::EnumFieldType fieldType,u32 size);

	private:
		vector<GFieldDef*> FieldDefList;
		GVectorDataLayer::EnumVectorLayerType LayerType;

	};

}

#endif