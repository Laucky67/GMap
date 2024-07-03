#include "GVectorDataInsertMDB.h"
#include "GVectorDataLayerMDB.h"
#include "GVectorDataSourceMDB.h"
#include "GGeometry.h"
#include "GSqlHelper.h"
namespace gmap{

	GVectorDataInsertMDB::GVectorDataInsertMDB(GVectorDataLayerMDB* pDataLayer)
	{
		DataLayerMDB=pDataLayer;
		wstring layerName = DataLayerMDB->GetName();
		wstring  tableName = wstring(L"GM_")+layerName;

		wstring insertSql = 
			wstring(L"insert into ")+tableName
		    +wstring(L"( GM_MINX,GM_MINY,GM_MAXX,GM_MAXY,GM_QCODE");

		wstring valueSql=
			wstring(L"values(?,?,?,?,?,?");

		DbCommand.CreateInstance(__uuidof(Command));
		DbCommand->ActiveConnection = DataLayerMDB->GetDataSourceMDB()->GetDBConnection();

		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_MINX"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_MINY"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_MAXX"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_MAXY"),GFieldDef::ftDouble,8);
		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_QCODE"),GFieldDef::ftBinary,12);
		
		for(int i=0;i<DataLayerMDB->GetFieldCount();i++)
		{
			GFieldDef* pDef = DataLayerMDB->GetFieldDef(i);
			GSqlHelper::AddSqlCommandParamter(DbCommand,pDef->GetName(),pDef->GetFieldType(),pDef->GetFieldSize());
			insertSql+=wstring(L",[")+pDef->GetName()+wstring(L"]");
			valueSql +=wstring(L",?");
		}
	  
		GSqlHelper::AddSqlCommandParamter(DbCommand,wstring(L"GM_GEOMETRY"),GFieldDef::ftBinary,1);


		insertSql+=wstring(L",GM_GEOMETRY)");
		valueSql+=wstring(L")");
		Sql = insertSql+valueSql;

		DbCommand->CommandText = _bstr_t(Sql.c_str());
		Geometry=NULL;
		GeomField=5+DataLayerMDB->GetFieldCount();

		MemFile = GMemWriteFile::Create();
	}

	GVectorDataInsertMDB::~GVectorDataInsertMDB(void)
	{
		if(Geometry)
			Geometry->Release();

		MemFile->Release();

	}

	//!设置几何字段
	void GVectorDataInsertMDB::SetGeometry(GGeometry* pGeometry)
	{
		
		if(Geometry)
		{
			Geometry->Release();
			Geometry = NULL;
		}
		Geometry = pGeometry;
		Geometry->AddRef();

		//build qcode
		if(pGeometry->GetGeometryType()!=GGeometry::GT_POINT)
		{
			QCode = DataLayerMDB->GetDataSourceMDB()->QCodeRasterizer->RasterizeGeometry(pGeometry);
			GQIndexRasterizer* ras = DataLayerMDB->GetDataSourceMDB()->QCodeRasterizer;

			GRectangle2dd box  = pGeometry->GetBoundingBox();
			double a = box.GetHeight()*box.GetWidth();
			double b =0;
			double cs = ras->GetLevelPixelSize(QCode.LEVEL);
			cs*=cs;
			for(int iy=0;iy<8;iy++)
			{
				for(int ix=0;ix<8;ix++)
				{
					if(GQIndexRasterizer::GetIndexPixelFlag(&QCode,ix,iy))
					{
						b+=cs;
					}
				}
			}
		}
	}
	
	//!设置字段的值
	void GVectorDataInsertMDB::SetFieldValue(u32 index,_variant_t& val)
	{
		DbCommand->Parameters->Item[(long)(index+5)]->Value = val;	
	}

	//!设置字段的值
	void GVectorDataInsertMDB::SetFieldValue(const wstring& fieldName,_variant_t& val)
	{
		DbCommand->Parameters->Item[_bstr_t(fieldName.c_str())]->Value = val;	
	}

	//!执行
	void GVectorDataInsertMDB::Execute()
	{
		GRectangle2dd box = Geometry->GetBoundingBox();
		DbCommand->Parameters->Item[0L]->Value = _variant_t(box.MinEdge.X);
		DbCommand->Parameters->Item[1L]->Value = _variant_t(box.MinEdge.Y);
		DbCommand->Parameters->Item[2L]->Value = _variant_t(box.MaxEdge.X);
		DbCommand->Parameters->Item[3L]->Value = _variant_t(box.MaxEdge.Y);
		
		SAFEARRAY* pQCodeArray = SafeArrayCreateVector(VT_UI1,0,sizeof(QCODE));
		memcpy(pQCodeArray->pvData,&QCode,sizeof(QCODE));
			
		VARIANT vQCodeArray;
		VariantInit(&vQCodeArray);
		vQCodeArray.vt = VT_UI1|VT_ARRAY;
		vQCodeArray.parray = pQCodeArray;

		DbCommand->Parameters->Item[4L]->Size =  sizeof(QCODE);
		DbCommand->Parameters->Item[4L]->Value = vQCodeArray;


		MemFile->Reset();
		GGeometry::SaveToStream(Geometry,MemFile);
		SAFEARRAY* pSafeArray = SafeArrayCreateVector(VT_UI1,0,MemFile->GetDataSize());
		memcpy(pSafeArray->pvData,MemFile->GetBuffer(),MemFile->GetDataSize());
		
		VARIANT vSafeArray;
		VariantInit(&vSafeArray);
		vSafeArray.vt = VT_UI1|VT_ARRAY;
		vSafeArray.parray = pSafeArray;
		
		DbCommand->Parameters->Item[GeomField]->Size =  MemFile->GetDataSize();
		DbCommand->Parameters->Item[GeomField]->Value = vSafeArray;

		DbCommand->Execute(NULL,NULL,adCmdText|adExecuteNoRecords);

		SafeArrayDestroy(pSafeArray);
		SafeArrayDestroy(pQCodeArray);
	}

}
