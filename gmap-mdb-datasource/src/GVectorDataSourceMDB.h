#ifndef __G_VECTOR_DATA_SOURCE_MDB_H__
#define __G_VECTOR_DATA_SOURCE_MDB_H__

#include "GVectorDataSource.h"
#include "adoInclude.h"
#include "GQindex.h"

#ifdef GMAPMDBDATASOURCE_EXPORTS
	#define GMAPMDBDATASOURCE_API __declspec(dllexport)
#else
	#define GMAPMDBDATASOURCE_API __declspec(dllimport)
#endif

namespace gmap{

	class GVectorDataLayerMDB;

	//!矢量数据源
	class GMAPMDBDATASOURCE_API GVectorDataSourceMDB:public GVectorDataSource
	{
	private:
		//!构造函数
		GVectorDataSourceMDB(const wstring& name,_ConnectionPtr dbConnection);

		//!
		void Open();

	public:

		~GVectorDataSourceMDB(void);

		//!关闭数据源
		virtual bool	Close();

		//!数据源的类型
		virtual EnumVectorDSType GetDSType();

		//!得到得到矢量图层的个数
		virtual u32	GetVectorLayerCount();

		//!得到图层
		virtual GVectorDataLayer*	GetLayer(u32 index);

		//!得到图层
		virtual GVectorDataLayer*	GetLayer(const wstring& name);

		//!创建图层
		GVectorDataLayer*			CreateLayer(const wstring& name,GVectorDataLayerDef* pLayerDef);

		//!删除Layer
		bool						DeleteLayer(const wstring& name);

		wstring						GetValidLayerName(const wstring& baseName);
	private:
		_ConnectionPtr GetDBConnection();

		friend	class GVectorDataLayerMDB;
		friend	class GVectorDataInsertMDB;
		friend  class GVectorDataQueryMDB;
	public:
		static GVectorDataSourceMDB* Create(const wstring& name,const wstring& file);

		static GVectorDataSourceMDB* Open(const wstring& name,const wstring& file);

	protected:
		wstring							ConnectionString;
		_ConnectionPtr					DbConnection;
		vector<GVectorDataLayerMDB*>	Layers;
		GQIndexRasterizer*				QCodeRasterizer;
	};

}


#endif