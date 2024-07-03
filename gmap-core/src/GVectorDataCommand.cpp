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

	//!����ֶ�
	void GVectorDataQuery::AddField(const wstring& name)
	{
		QueryFieldList.push_back(name);
		Dirty = true;
	}

	//!�õ��ֶθ���
	u32 GVectorDataQuery::GetFieldCount()
	{
		return QueryFieldList.size();
	}

	//!����SQL
	void GVectorDataQuery::SetSqlWhere(const wstring& sql)
	{
		SqlWhere = sql;
		
		Dirty = true;
	}

	//!���ð�Χ��
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
