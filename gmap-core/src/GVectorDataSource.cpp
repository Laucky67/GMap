#include "GVectorDataSource.h"
namespace gmap{

	//!���캯��
	GVectorDataSource::GVectorDataSource(const wstring& name)
	{
		Name = name;
	}

	//!��������
	GVectorDataSource::~GVectorDataSource(void)
	{
		
	}

	//!����Դ����
	const wstring& GVectorDataSource::GetName()
	{
		return Name;
	}

	//!�Ƿ��״̬
	GVectorDataSource::EnumVectorDSStatus	GVectorDataSource::GetStatus()
	{
		return Status;
	}
}
