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

	//!ʸ������Դ
	class GMAPMDBDATASOURCE_API GVectorDataSourceMDB:public GVectorDataSource
	{
	private:
		//!���캯��
		GVectorDataSourceMDB(const wstring& name,_ConnectionPtr dbConnection);

		//!
		void Open();

	public:

		~GVectorDataSourceMDB(void);

		//!�ر�����Դ
		virtual bool	Close();

		//!����Դ������
		virtual EnumVectorDSType GetDSType();

		//!�õ��õ�ʸ��ͼ��ĸ���
		virtual u32	GetVectorLayerCount();

		//!�õ�ͼ��
		virtual GVectorDataLayer*	GetLayer(u32 index);

		//!�õ�ͼ��
		virtual GVectorDataLayer*	GetLayer(const wstring& name);

		//!����ͼ��
		GVectorDataLayer*			CreateLayer(const wstring& name,GVectorDataLayerDef* pLayerDef);

		//!ɾ��Layer
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