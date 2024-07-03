#include "GVectorDataSourceMDB.h"
#include "GVectorDataLayerDef.h"
#include "GVectorDataLayerMDB.h"
#include "GSqlHelper.h"
#include "GVectorDataSourceMDB_Api.h"
namespace gmap{

	GVectorDataSourceMDB::GVectorDataSourceMDB(const wstring& name,_ConnectionPtr dbConnection)
		:GVectorDataSource(name)
	{
		DbConnection = dbConnection;
		QCodeRasterizer = new GQIndexRasterizer();
		QCodeRasterizer->Initialize(GRectangle2dd(60,0,140,80));
	}

	GVectorDataSourceMDB::~GVectorDataSourceMDB(void)
	{
		delete QCodeRasterizer;
	}

	//!关闭数据源
	bool	GVectorDataSourceMDB::Close()
	{	
		for(int i=0;i<Layers.size();i++)
		{
			Layers[i]->Release();
		}
		DbConnection->Close();
		return true;
	}

	//!数据源的类型
	GVectorDataSource::EnumVectorDSType GVectorDataSourceMDB::GetDSType()
	{
		return GVectorDataSource::VDT_MDB;
	}

	//!得到得到矢量图层的个数
	u32	GVectorDataSourceMDB::GetVectorLayerCount()
	{
		return Layers.size();
	}

	//!得到图层
	GVectorDataLayer* GVectorDataSourceMDB::GetLayer(u32 index)
	{
		return Layers[index];
	}

	//!得到图层
	GVectorDataLayer* GVectorDataSourceMDB::GetLayer(const wstring& name)
	{
		for(int i=0;i<Layers.size();i++)
		{
			if(Layers[i]->GetName()==name)
				return Layers[i];
		}
		return NULL;
	}

	//!创建图层
	GVectorDataLayer* GVectorDataSourceMDB::CreateLayer(const wstring& name,GVectorDataLayerDef* pLayerDef)
	{
		DbConnection->BeginTrans();

		wstring layerName =GetValidLayerName(name);
		wstring tableName =wstring(L"GM_")+layerName;


		wstring sql=wstring(L"Create Table ")
			+tableName+wstring(L" (")
			+wstring(L" GM_ID int Identity(1, 1),");

		sql+=wstring(L" GM_MINX float,");
		sql+=wstring(L" GM_MINY float,");
		sql+=wstring(L" GM_MAXX float,");
		sql+=wstring(L" GM_MAXY float,");
		sql+=wstring(L" GM_IK   int,");
		sql+=wstring(L" GM_QCODE binary(16),");
		sql+=wstring(L" GM_GEOMETRY image");

		int count = pLayerDef->GetFieldCount();
		for(int i=0;i<count;i++)
		{
			GFieldDef* pDef = pLayerDef->GetFieldDef(i);
			wstring field = GSqlHelper::GetlSqStatementOfField(pDef->GetName(),pDef->GetFieldType(),pDef->GetFieldSize());
			sql+=wstring(L" ,")+field;
		}

		sql+=wstring(L")");
		
		
		HRESULT hr;
		hr = DbConnection->Execute(_bstr_t(sql.c_str()),NULL,adCmdText|adExecuteNoRecords);

		_CommandPtr command(__uuidof(Command));
		command->ActiveConnection = DbConnection;

		sql=wstring(L"Insert Into GM_LAYER_REGISTER_TABLE(GM_NAME,GM_TABLE_NAME,GM_GEOMETRY_TYPE,GM_SRID,GM_MINX,GM_MINY,GM_MAXX,GM_MAXY)");
		sql+=wstring(L"Values(?,?,?,?,?,?,?,?)");
		command->CommandText = _bstr_t(sql.c_str());

		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_NAME"),GFieldDef::ftLongVarWChar,255);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_TABLE_NAME"),GFieldDef::ftLongVarWChar,255);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_GEOMETRY_TYPE"),GFieldDef::ftInteger,4);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_SRID"),GFieldDef::ftLongVarWChar,255);

		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MINX"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MINY"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MAXX"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MAXY"),GFieldDef::ftDouble,8);

		ParametersPtr parameters = command->Parameters;
		parameters->Item[0L]->Value = _variant_t(_bstr_t(layerName.c_str()));
		parameters->Item[1L]->Value = _variant_t(_bstr_t(tableName.c_str()));
		parameters->Item[2L]->Value = _variant_t((int)pLayerDef->GetLayerType());
		parameters->Item[3L]->Value = _variant_t(_bstr_t(L"unknown"));
		parameters->Item[4L]->Value = _variant_t(0.0);
		parameters->Item[5L]->Value = _variant_t(0.0);
		parameters->Item[6L]->Value = _variant_t(0.0);
		parameters->Item[7L]->Value = _variant_t(0.0);

		command->Execute(NULL,NULL,adCmdText|adExecuteNoRecords);

		DbConnection->CommitTrans();

		GRectangle2dd bounds(0,0,0,0);
		GVectorDataLayerMDB* pLayer = new GVectorDataLayerMDB(this,layerName,bounds,pLayerDef);
		Layers.push_back(pLayer);

		return pLayer;
	}

	//!删除Layer
	bool	GVectorDataSourceMDB::DeleteLayer(const wstring& name)
	{
		return false;
	}
	/*
	表名:GM_LAYER_REGISTER_TABLE		
	GM_LAYER_ID	  :int
	GM_LAYER_NAME :wchar(255)
	GM_LAYER_TABLE_NAME:wchar(255)
	GM_LAYER_SPATIAL_INDEX_TYPE:bool
	GM_LAYER_SRID:int
	GM_LAYER_MINX:double
	GM_LAYER_MINY:double
	GM_LAYER_MAXX:double
	GM_LAYER_MAXY:double
	*/
	wstring g_conn_header(L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	//wstring g_conn_header(L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=");
	GVectorDataSourceMDB* GVectorDataSourceMDB::Create(const wstring& name,const wstring& file)
	{
		_ConnectionPtr dbConnection;
		dbConnection.CreateInstance(__uuidof(Connection));

		wstring connStr=g_conn_header  //L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
									 +file;
		HRESULT hr = dbConnection->Open(_bstr_t(connStr.c_str()),_bstr_t(L""),_bstr_t(L""),ADODB::adConnectUnspecified);
		if(SUCCEEDED(hr))
		{
			if(dbConnection->State == adStateOpen)
			{
				_RecordsetPtr rs = dbConnection->OpenSchema(adSchemaTables);
				_bstr_t registerTbName(L"GM_LAYER_REGISTER_TABLE");
				bool bCreate=true;
				while(!rs->AdoEOF)
				{
					_variant_t val = rs->Fields->Item["TABLE_NAME"]->Value;
					//val.intVal;

					//int ival = (int)val;

					_bstr_t tbName(val);
					if(tbName==registerTbName)
					{
						bCreate=false;
						break;
					}
					rs->MoveNext();
				}

				if(bCreate)
				{
					wstring sql=
						wstring(L"CREATE TABLE GM_LAYER_REGISTER_TABLE (")
						+	wstring(L"GM_ID int Identity(1, 1),")
						+	wstring(L"GM_NAME nvarchar(255) ,")
						+	wstring(L"GM_TABLE_NAME nvarchar(255),")
						+	wstring(L"GM_GEOMETRY_TYPE int,")
						+	wstring(L"GM_SRID String,")
						+	wstring(L"GM_MINX float,")
						+	wstring(L"GM_MINY float,")
						+	wstring(L"GM_MAXX float,")
						+	wstring(L"GM_MAXY float,")
						+	wstring(L"GM_MEMO IMAGE")
						+	wstring(L")");

					_bstr_t bstrSql(sql.c_str());

					hr = dbConnection->Execute(bstrSql,NULL,adCmdText|adExecuteNoRecords);
					if(FAILED(hr))
					{
						return NULL;
					}
				}

				GVectorDataSourceMDB* pDataSource = new GVectorDataSourceMDB(name,dbConnection);
				pDataSource->Open();

				return pDataSource;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}

	}

	GVectorDataSourceMDB* GVectorDataSourceMDB::Open(const wstring& name,const wstring& file)
	{
		_ConnectionPtr dbConnection;
		dbConnection.CreateInstance(__uuidof(Connection));

		wstring connStr=
			g_conn_header
			+file;

		HRESULT hr = dbConnection->Open(_bstr_t(connStr.c_str()),_bstr_t(L""),_bstr_t(L""),adConnectUnspecified);
		if(SUCCEEDED(hr))
		{
			if(dbConnection->State == adStateOpen)
			{
				_RecordsetPtr rs = dbConnection->OpenSchema(adSchemaTables);

				_bstr_t registerTbName(L"GM_LAYER_REGISTER_TABLE");
				bool bCreated=false;

				while(!rs->AdoEOF)
				{

					_bstr_t tbName( rs->Fields->Item["TABLE_NAME"]->Value);
					if(tbName==registerTbName)
					{
						bCreated=true;
						break;
					}
					rs->MoveNext();
				}
				if(bCreated)
				{
					GVectorDataSourceMDB* pDataSource = new GVectorDataSourceMDB(name,dbConnection);
					pDataSource->Open();
					return pDataSource;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}

	void GVectorDataSourceMDB::Open()
	{
		wstring sql=
			wstring(L"SELECT GM_ID,GM_NAME,GM_TABLE_NAME,GM_GEOMETRY_TYPE,GM_SRID,GM_MINX,GM_MINY,GM_MAXX,GM_MAXY FROM GM_LAYER_REGISTER_TABLE");

		_RecordsetPtr layerRs = DbConnection->Execute(_bstr_t(sql.c_str()),NULL,adCmdText);

		while(!layerRs->AdoEOF)
		{		
			 u32		id		= layerRs->Fields->Item[0L]->Value;
			_bstr_t layerName(layerRs->Fields->Item[1L]->Value);	
			_bstr_t tableName(layerRs->Fields->Item[2L]->Value);
			u32			geometryType = layerRs->Fields->Item[3L]->Value;
			_bstr_t		srid (layerRs->Fields->Item[4L]->Value);

			f64 minx	= layerRs->Fields->Item[5L]->Value;
			f64 miny	= layerRs->Fields->Item[6L]->Value;
			f64 maxx	= layerRs->Fields->Item[7L]->Value;
			f64 maxy	= layerRs->Fields->Item[8L]->Value;

			
			wstring sql=wstring(L"select * from ")+wstring((wchar_t*)tableName)+wstring(L" where 0>1");
			_RecordsetPtr tableRs = DbConnection->Execute(_bstr_t(sql.c_str()),NULL,adCmdText);

			long fieldCount = tableRs->Fields->Count;

			GVectorDataLayerDef* pLayerDef = new GVectorDataLayerDef((GVectorDataLayer::EnumVectorLayerType)geometryType);
			for(long i=6;i<fieldCount;i++)
			{
				_bstr_t	fieldName =
					tableRs->Fields->Item[i]->Name; 

				GFieldDef::EnumFieldType fieldType = 
					(GFieldDef::EnumFieldType)((u32)tableRs->Fields->Item[i]->Type);

				u32 size = 
					tableRs->Fields->Item[i]->DefinedSize;

				pLayerDef->AddField(wstring((wchar_t*)fieldName),fieldType,size);
			}
			
			GVectorDataLayerMDB* pDataLayer = new GVectorDataLayerMDB(this,
													wstring((wchar_t*)layerName),
													GRectangle2dd(minx,miny,maxx,maxy),
													pLayerDef);
			pLayerDef->Release();
			Layers.push_back(pDataLayer);

			layerRs->MoveNext();
		}
	}

	_ConnectionPtr GVectorDataSourceMDB::GetDBConnection()
	{
		return DbConnection;
	}

	wstring GVectorDataSourceMDB::GetValidLayerName(const wstring& baseName)
	{
		wchar_t fixName[16];
		memset(fixName,0,sizeof(wchar_t)*16);
		u32 nFix=0;
		while(1)
		{	
			wstring name=baseName;
			if(nFix>0)
			{
				_itow_s(nFix,fixName,10);
				name+=wstring(fixName);
			}
			wstring sqlQuery= wstring(L"select * from GM_LAYER_REGISTER_TABLE where GM_NAME='")
				+name
				+wstring(L"'");
			_RecordsetPtr rsRegTb=DbConnection->Execute(_bstr_t(sqlQuery.c_str()),NULL,adCmdText);

			if(rsRegTb->AdoEOF==VARIANT_TRUE)
			{
				return name;
			}
			nFix++;
		}
		return baseName;
	}

	//////////////////////////////////////////////////////////////////////////////////////
	GVectorDataSource* CreateVectorDataSourceMDB(const wstring& name, const wstring& file)
	{
		return GVectorDataSourceMDB::Create(name, file);
	}

	GVectorDataSource* OpenVectorDataSourceMDB(const wstring& name, const wstring& file)
	{
		return GVectorDataSourceMDB::Open(name, file);
	}
}