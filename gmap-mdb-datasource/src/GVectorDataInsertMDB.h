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
		
		//!���ü����ֶ�
		virtual void SetGeometry(GGeometry* pGeometry);


		//!�����ֶε�ֵ
		virtual void SetFieldValue(u32 index,_variant_t& val);

		//!�����ֶε�ֵ
		virtual void SetFieldValue(const wstring& fieldName,_variant_t& val);

		//!ִ��
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