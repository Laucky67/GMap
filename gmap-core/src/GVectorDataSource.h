#ifndef __G_VECTOR_DATA_SOURCE_H__
#define __G_VECTOR_DATA_SOURCE_H__


#include "GCoreBase.h"
#include "GUnknown.h"
#include "GGeometry.h"
#include "GVectorDataLayer.h"

namespace gmap{

	class GVectorDataLayer;
	class GFieldDef;
	class GVectorDataLayerDef;

	//!矢量数据源
	class GMAPCORE_API GVectorDataSource:public GUnknown
	{
	public:
		enum EnumVectorDSType
		{
			VDT_UNKNOWN,
			VDT_MDB
		};

		enum EnumVectorDSStatus
		{
			VDS_UNKNOWN,
			VDS_OPEN,
			VDS_CLOSED,
			VDS_ERROR
		};
	public:
		//!构造函数
		GVectorDataSource(const wstring& name);

		//!析构函数
		~GVectorDataSource(void);

		//!数据源名称
		virtual const wstring& GetName();

		//!是否打开状态
		virtual EnumVectorDSStatus	GetStatus();

		//!关闭数据源
		virtual bool	Close()=0;

		//!数据源的类型
		virtual EnumVectorDSType GetDSType()=0;

		//!得到得到矢量图层的个数
		virtual u32	GetVectorLayerCount()=0;

		//!得到图层
		virtual GVectorDataLayer* GetLayer(u32 index)=0;

		//!得到图层
		virtual GVectorDataLayer* GetLayer(const wstring& name)=0;
		
		//!创建图层
		virtual GVectorDataLayer* CreateLayer(const wstring& name,GVectorDataLayerDef* pLayerDef){return NULL;}

		//!删除Layer
		bool	DeleteLayer(const wstring& name){return false;}
	protected:
		wstring Name;
		EnumVectorDSStatus	Status;
	};

}

#endif