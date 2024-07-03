#ifndef __G_VECTOR_DATA_SOURCE_MDB_API_H__
#define __G_VECTOR_DATA_SOURCE_MDB_API_H__

#include "GVectorDataSource.h"

#ifdef GMAPMDBDATASOURCE_EXPORTS
	#define GMAPMDBDATASOURCE_API __declspec(dllexport)
#else
	#define GMAPMDBDATASOURCE_API __declspec(dllimport)
#endif

namespace gmap {
	GMAPMDBDATASOURCE_API
	GVectorDataSource* 		 CreateVectorDataSourceMDB(const wstring& name, const wstring& file);
	GMAPMDBDATASOURCE_API
	GVectorDataSource*		 OpenVectorDataSourceMDB(const wstring& name, const wstring& file);
}

#endif
