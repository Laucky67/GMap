#ifndef __G_VECTOR_DATA_LAYER_DEF_H__
#define __G_VECTOR_DATA_LAYER_DEF_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GFieldDef.h"
#include "GVectorDataLayer.h"

namespace gmap{

	//!图层定义
	class GMAPCORE_API GVectorDataLayerDef:public GUnknown
	{
	public:
		GVectorDataLayerDef(GVectorDataLayer::EnumVectorLayerType layerType);

		~GVectorDataLayerDef(void);

		//!图层类型
		GVectorDataLayer::EnumVectorLayerType GetLayerType();

		//!得到字段的个数
		u32 GetFieldCount();

		//!得到字段的定义
		GFieldDef* GetFieldDef(u32 index);

		//!添加字段
		void AddField(const wstring& name,GFieldDef::EnumFieldType fieldType,u32 size);

	private:
		vector<GFieldDef*> FieldDefList;
		GVectorDataLayer::EnumVectorLayerType LayerType;

	};

}

#endif