#include "GVectorDataSource.h"
namespace gmap{

	//!构造函数
	GVectorDataSource::GVectorDataSource(const wstring& name)
	{
		Name = name;
	}

	//!析构函数
	GVectorDataSource::~GVectorDataSource(void)
	{
		
	}

	//!数据源名称
	const wstring& GVectorDataSource::GetName()
	{
		return Name;
	}

	//!是否打开状态
	GVectorDataSource::EnumVectorDSStatus	GVectorDataSource::GetStatus()
	{
		return Status;
	}
}
