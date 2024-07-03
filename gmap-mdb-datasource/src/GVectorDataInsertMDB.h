#ifndef __GVectorDataInsertMDB_H__
#define __GVectorDataInsertMDB_H__
#include "GCoreBase.h"
#include "GVectorDataCommand.h"
#include "AdoInclude.h"
#include "GQindex.h"

namespace gmap{

	class GVectorDataLayerMDB;

	class GVectorDataInsertMDB:public GVectorDataInsert
	{
	public:
		GVectorDataInsertMDB(GVectorDataLayerMDB* pDataLayer);

		~GVectorDataInsertMDB(void);
		
		//!设置几何字段
		virtual void SetGeometry(GGeometry* pGeometry);


		//!设置字段的值
		virtual void SetFieldValue(u32 index,_variant_t& val);

		//!设置字段的值
		virtual void SetFieldValue(const wstring& fieldName,_variant_t& val);

		//!执行
		virtual void Execute();
	private:
		GVectorDataLayerMDB* DataLayerMDB;
		_CommandPtr			 DbCommand;
		GGeometry*			 Geometry;
		QCODE				 QCode;
		wstring				 Sql;
		GMemWriteFile*		 MemFile;
		long				 GeomField;

	};

}


#endif