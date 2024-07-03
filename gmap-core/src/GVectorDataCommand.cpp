#include "GVectorDataCommand.h"
#include "GVectorDataLayer.h"
namespace gmap{
		
	GVectorDataQuery::GVectorDataQuery(bool geometryField)
	{
		QueryGeometryField = geometryField;
		SpatialFilter = NULL;
		SpatialFilterType = GVectorDataQuery::SFT_NULL;
	}

	GVectorDataQuery::~GVectorDataQuery()
	{
		if(SpatialFilter)
		{
			SpatialFilter->Release();
		}
	}

	//!添加字段
	void GVectorDataQuery::AddField(const wstring& name)
	{
		QueryFieldList.push_back(name);
		Dirty = true;
	}

	//!得到字段个数
	u32 GVectorDataQuery::GetFieldCount()
	{
		return QueryFieldList.size();
	}

	//!设置SQL
	void GVectorDataQuery::SetSqlWhere(const wstring& sql)
	{
		SqlWhere = sql;
		
		Dirty = true;
	}

	//!设置包围盒
	void GVectorDataQuery::SetSpatialFilter(EnumSpatialFilterType filterType,GGeometry* filter)
	{
		SpatialFilterType = filterType;
				
		if(SpatialFilter)
		{
			SpatialFilter->Release();
			SpatialFilter = NULL;
		}
		SpatialFilter = filter;
		SpatialFilter->AddRef();
		Dirty = true;
	}
}
