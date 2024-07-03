#include "stdafx.h"
#include "ogrsf_frmts.h"
#include "GOGRImporter.h"
#include "GFieldDef.h"
#include "atlconv.h"
#include "GVectorDataCommand.h"
#include "GMultiGeometry.h"
#include "GPoint.h"
#include "GLineString.h"
#include "GPolygon.h"
#include "GVectorDataLayerDef.h"
#include "IProgressUI.h"
#include <set>

namespace gmap{

	GFieldDef::EnumFieldType GetFieldTypeFromOGRType(OGRFieldType tp)
	{
		switch((u32)tp)
		{
		case	OFTInteger:return GFieldDef::ftInteger;
		case	OFTIntegerList: GFieldDef::ftEmpty;
		case	OFTReal :return GFieldDef::ftDouble;
		case	OFTRealList :return GFieldDef::ftEmpty;
		case	OFTString :return GFieldDef::ftVarChar;
		case	OFTStringList:return GFieldDef::ftEmpty;
		case	OFTWideString :return GFieldDef::ftVarWChar;
		case	OFTWideStringList:return GFieldDef::ftEmpty;
		case	OFTBinary:return GFieldDef::ftBinary;
		case	OFTDate:return GFieldDef::ftDBDate;
		case	OFTTime:return GFieldDef::ftDBTime;
		case	OFTDateTime :return GFieldDef::ftDate;
		default:return  GFieldDef::ftEmpty;

		}
	}

	std::string UTF8_To_string(const std::string & str)
	{
		int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		//一定要加1，不然会出现尾巴
		memset(pwBuf, 0, nwLen * 2 + 2);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		memset(pBuf, 0, nLen + 1);
		WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string strRet = pBuf;
		delete[] pBuf;
		delete[] pwBuf;
		pBuf = NULL;
		pwBuf = NULL;
		return strRet;
	}

	//从OGR转换Variant
	_variant_t GetOGRFieldValue(OGRFeature* feature,int index,OGRFieldType dt,bool utf8)
	{
		switch(dt)
		{
		case OFTInteger:return _variant_t(feature->GetFieldAsInteger(index));
		case OFTReal:return  _variant_t(feature->GetFieldAsDouble(index));
		case OFTString:
		{
			const char* sval = feature->GetFieldAsString(index);
			string astring(sval);
			if(utf8)
				astring = UTF8_To_string(sval);
			_bstr_t bstr(astring.c_str());
			return _variant_t(bstr);
		};
		case OFTDate:
		case OFTTime:
		case OFTDateTime:			
			{
				int y, m, d, h, mt, s, tz;
				feature->GetFieldAsDateTime(index,&y,&m,&d,&h,&mt,&s,&tz);
				_SYSTEMTIME stm = { y,m,0,d,h,mt,s,0 };
				double vDate;
				SystemTimeToVariantTime(&stm,&vDate);
				return _variant_t(vDate,VT_DATE);
			}
		default:
			return vtMissing;
		}
	}

	GGeometry::EnumGeometryType GetGeometryTypeFromOGRGeometryType(OGRwkbGeometryType gtype)
	{
		 gtype = (OGRwkbGeometryType)(0xF & gtype);

		 if (gtype == wkbPoint)
		 {
			 return GGeometry::GT_POINT;
		 }
		 else if (gtype == wkbLineString)
		 {
			 return GGeometry::GT_LINE_STRING;
		 }
		 else if (gtype == wkbPolygon)
		 {
			 return GGeometry::GT_POLYGON;
		 }
		 else if (gtype == wkbMultiPoint || gtype == wkbMultiLineString
			 || gtype == wkbMultiPolygon || gtype == wkbGeometryCollection)
		 {
			 return GGeometry::GT_COLLECTION;
		 }
		 return GGeometry::GT_UNKNOWN;
	}

	GGeometry* CreateFromOGRGeometry(OGRGeometry* pOGRGeometry)
	{
		OGRwkbGeometryType gtype = (OGRwkbGeometryType)(0xF&pOGRGeometry->getGeometryType());
	
		if(gtype==wkbPoint)
		{
			OGRPoint* point = (OGRPoint*)pOGRGeometry;
			return GPoint::Create(point->getX(),point->getY());
		}
		else if(gtype==wkbLineString)
		{
			OGRLineString* lineString= (OGRLineString*)pOGRGeometry;
			int pointCount = lineString->nPointCount;
			OGRRawPoint* rawPoints = lineString->paoPoints;
			return GLineString::Create(pointCount,(GCoordinate*)rawPoints);
		}
		else if(gtype==wkbPolygon)
		{
			OGRPolygon* polygon =(OGRPolygon*)pOGRGeometry;

			OGRLinearRing* ring = polygon->getExteriorRing();
			int pointCount = ring->nPointCount;
			OGRRawPoint* rawPoints = ring->paoPoints;
			GPolygon* gpolygon= GPolygon::Create(pointCount,(GCoordinate*)rawPoints);

			for(int i=0;i<polygon->getNumInteriorRings();i++)
			{
				ring = polygon->getInteriorRing(i);
				pointCount = ring->nPointCount;
				rawPoints = ring->paoPoints;
				GLinearRing* glinearRing= GLinearRing::Create(pointCount,(GCoordinate*)rawPoints);
				gpolygon->AddInternalRing(glinearRing,false);
				glinearRing->Release();
			}

			return gpolygon;
		}
		else if(gtype==wkbMultiPoint||gtype==wkbMultiLineString
			   ||gtype==wkbMultiPolygon||gtype==wkbGeometryCollection)
		{
			OGRGeometryCollection* geoms = (OGRGeometryCollection*)pOGRGeometry;
			GMultiGeometry* multiGeom = GMultiGeometry::Create();
			for(int i=0;i<geoms->getNumGeometries();i++)
			{
				GGeometry* pGeometry=CreateFromOGRGeometry(geoms->getGeometryRef(i));
				multiGeom->AppendGeometry(pGeometry,false);
				pGeometry->Release();
			}
			return multiGeom;
		}
		return NULL;
	}


	void GOGRImporter::RegisterAll()
	{
		OGRRegisterAll();
	}

	GVectorDataLayer* GOGRImporter::Import(const wstring& name,OGRLayer* pLayer,GVectorDataSource* pDataSource,IProgressUI* pUI,bool utf8)
	{
		OGRFeatureDefn* pFeatureDefn= pLayer->GetLayerDefn();
		GGeometry::EnumGeometryType gt = GetGeometryTypeFromOGRGeometryType(pFeatureDefn->GetGeomType());
		GVectorDataLayerDef* pLayerDef = new GVectorDataLayerDef((GVectorDataLayer::EnumVectorLayerType)gt);
		
		u32 nFieldCount = pFeatureDefn->GetFieldCount();
		vector<u32> fieldIndices;
		vector<OGRFieldType> fieldTypes;
		
		USES_CONVERSION;
		for(u32 iField=0;iField<nFieldCount;iField++)
		{
			OGRFieldDefn* pFdd=pFeatureDefn->GetFieldDefn(iField);
			wstring name(A2W(pFdd->GetNameRef()));
			u32 size = pFdd->GetWidth();
			GFieldDef::EnumFieldType fieldType = GetFieldTypeFromOGRType(pFdd->GetType());
			if(fieldType!=GFieldDef::ftEmpty)
			{
				pLayerDef->AddField(name,fieldType,size);
				fieldTypes.push_back(pFdd->GetType());
				fieldIndices.push_back(iField);
			}
		}

		GVectorDataLayer* pDataLayer = pDataSource->CreateLayer(name,pLayerDef);
		GVectorDataInsert* pInsert = pDataLayer->CreateInsertCommand();
		
		OGRFeature *pFeature=NULL;
		pLayer->ResetReading();
		long nId =1;
		u32 total = pLayer->GetFeatureCount();
		if(pUI)
		{
			pUI->OnSetTotal(L"IMPORT_FEATURE",total);
		}
		nFieldCount = fieldIndices.size();

		u32 counter=0;
		while((pFeature=pLayer->GetNextFeature())!=NULL)
		{
			OGRGeometry* pGeometry = pFeature->GetGeometryRef();
			GGeometry*   gGeometry = CreateFromOGRGeometry(pGeometry);
			pInsert->SetGeometry(gGeometry);

			for(u32 iField=0;iField<nFieldCount;iField++)
			{
				_variant_t val2 = GetOGRFieldValue(pFeature,fieldIndices[iField],fieldTypes[iField],utf8);
				pInsert->SetFieldValue(iField,val2);				
			}
			pInsert->Execute();
			gGeometry->Release();
			counter++;
			if(pUI)
			{
				pUI->OnProgress(L"IMPORT_FEATURE",counter);
			}
		}

		return pDataLayer;
	}


	OGRGeometry* CreateOGRFromGMapGeometry(GGeometry* pGeometry)
	{
		if(pGeometry->GetGeometryType()==GGeometry::GT_LINE_STRING)
		{
			OGRLineString ls;

			GLineString* pLineString = (GLineString* )pGeometry;
			int num = pLineString->GetCoordinateCount();
			GCoordinate* pCoords = new GCoordinate[num];
			pLineString->GetCoordinates(pCoords);

			ls.setPoints(num,(OGRRawPoint*)pCoords);

			delete[] pCoords;

			return ls.clone();
		}
		else if(pGeometry->GetGeometryType()==GGeometry::GT_POLYGON)
		{
			OGRPolygon polygon;
						
			OGRLinearRing ring;

			GPolygon* pPolygon = (GPolygon* )pGeometry;
			int num = pPolygon->GetExternalRing()->GetCoordinateCount();
			GCoordinate* pCoords = new GCoordinate[num];
			pPolygon->GetExternalRing()->GetCoordinates(pCoords);

			ring.setPoints(num,(OGRRawPoint*)pCoords);

			polygon.addRing(&ring);
			delete[] pCoords;

			return polygon.clone();			
		}
		else
		{
			return NULL;
		}
	}

	GVectorDataLayer* GOGRImporter::ExportMiTab(GVectorDataLayer* pSrcLayer,char* file,char* name)
	{
		const char *pszDriverName = "Mapinfo File";

		OGRSFDriver *poDriver;

		//OGRRegisterAll();

		poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
			pszDriverName );
		if( poDriver == NULL )
		{
			printf( "%s driver not available.\n", pszDriverName );
			exit( 1 );
		}

		OGRDataSource *poDS;

		poDS = poDriver->CreateDataSource(file, NULL );

		if( poDS == NULL )
		{
			printf( "Creation of output file failed.\n" );
			exit( 1 );
		}

		OGRLayer *poLayer;

		 OGRSpatialReference oSRS;
		         oSRS.SetGeogCS( "My geographic coordinate system",
                        "BEIJING54", 
                        "Krassovsky  Spheroid", 
                         6378245, 298.257223563, 
                        "Greenwich", 0.0, 
                        "degree", 0.0174532925199433 );

		//oSRS.SetWellKnownGeogCS( "WGS84" );

		poLayer = poDS->CreateLayer(name,&oSRS);
		if( poLayer == NULL )
		{
			printf( "Layer creation failed.\n" );
			exit( 1 );
		}

		OGRFieldDefn oField( "IdName", OFTString );
		oField.SetWidth(32);

		if( poLayer->CreateField( &oField ) != OGRERR_NONE )
		{
			printf( "Creating Name field failed.\n" );
			exit( 1 );
		}


		GVectorDataQuery* pQuery=	pSrcLayer->CreateQueryCommand(true);
		GVectorDataQueryResult* pResult = pQuery->Execute();

		while(!pResult->IsEOF())
		{	
			OGRFeature* poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn() );
			GGeometry* pGeometryGM = pResult->GetGeometry();
			OGRGeometry* pGeometryORG = CreateOGRFromGMapGeometry(pGeometryGM);

			poFeature->SetGeometry(pGeometryORG);

			if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
			{
				printf( "Failed to create feature in shapefile.\n" );
				exit( 1 );
			}

			OGRFeature::DestroyFeature(poFeature);
			OGRGeometryFactory::destroyGeometry(pGeometryORG);			
			pResult->MoveNext();
		}
		OGRDataSource::DestroyDataSource( poDS );
	}
}