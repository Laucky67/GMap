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

	//!ʸ������Դ
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
		//!���캯��
		GVectorDataSource(const wstring& name);

		//!��������
		~GVectorDataSource(void);

		//!����Դ����
		virtual const wstring& GetName();

		//!�Ƿ��״̬
		virtual EnumVectorDSStatus	GetStatus();

		//!�ر�����Դ
		virtual bool	Close()=0;

		//!����Դ������
		virtual EnumVectorDSType GetDSType()=0;

		//!�õ��õ�ʸ��ͼ��ĸ���
		virtual u32	GetVectorLayerCount()=0;

		//!�õ�ͼ��
		virtual GVectorDataLayer* GetLayer(u32 index)=0;

		//!�õ�ͼ��
		virtual GVectorDataLayer* GetLayer(const wstring& name)=0;
		
		//!����ͼ��
		virtual GVectorDataLayer* CreateLayer(const wstring& name,GVectorDataLayerDef* pLayerDef){return NULL;}

		//!ɾ��Layer
		bool	DeleteLayer(const wstring& name){return false;}
	protected:
		wstring Name;
		EnumVectorDSStatus	Status;
	};

}

#endif