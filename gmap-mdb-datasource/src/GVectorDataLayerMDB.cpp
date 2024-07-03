#include "GVectorDataLayerMDB.h"
#include "GVectorDataSourceMDB.h"
#include "GVectorDataLayerDef.h"
#include "GVectorDataInsertMDB.h"
#include "GVectorDataQueryMDB.h"
#include "adoInclude.h"
#include "GRtVsitor.h"
#include "GRtLeaf.h"
#include "IProgressUI.h"
#include "GSqlHelper.h"

namespace gmap{

	GVectorDataLayerMDB::GVectorDataLayerMDB(GVectorDataSourceMDB* pDataSource,const wstring& name,const GRectangle2dd& bounds,GVectorDataLayerDef* pLayerDef)
		:GVectorDataLayer(name)
	{
		_ConnectionPtr dbConnection = pDataSource->GetDBConnection();
		LayerType = pLayerDef->GetLayerType();

		for(int i=0;i<pLayerDef->GetFieldCount();i++)
		{
			GFieldDef* pDef = pLayerDef->GetFieldDef(i);			
			FieldDefList.push_back(pDef);
			pDef->AddRef();
		}
		DataSource = pDataSource;

		BoundingBox = bounds;
		DefaultMapQuery = NULL;
		RTree = NULL;

		RtMinChildCount=0;
		RtMaxChildCount=0;
		RtMinItemCount=0;
		RtMaxItemCount=0;
		RtNextId = 0;
	}

	GVectorDataLayerMDB::~GVectorDataLayerMDB(void)
	{
		if(DefaultMapQuery)
			DefaultMapQuery->Release();

		if(RTree)
		{	
			RTree->uninitialize();
			delete RTree;
		}

		for(vector<GQRtItem*>::iterator it= QItems.begin();it!=QItems.end();it++)
		{
			delete (*it);
		}
	}


	GVectorDataSource* GVectorDataLayerMDB::GetDataSource()
	{
		return DataSource;
	}


	GVectorDataSourceMDB* GVectorDataLayerMDB::GetDataSourceMDB()
	{
		return DataSource;
	}

	//!图层类型
	GVectorDataLayer::EnumVectorLayerType GVectorDataLayerMDB::GetLayerType()
	{
		return LayerType;
	}

	//!得到包围盒
	const GRectangle2dd& GVectorDataLayerMDB::GetBoundingBox()
	{
		return BoundingBox;
	}

	
	const u32 _RtMinChildCount = 8;
	const u32 _RtMaxChildCount =16;
	const u32 _RtMinItemCount  = 8;
	const u32 _RtMaxItemCount  =16;


	class GmIKWriter:public rtree::IRtVisitor
	{
		_CommandPtr Command;
		IProgressUI* UI;
		u32 id;
	public:
		GmIKWriter(_CommandPtr command,IProgressUI* pUI)
		{
			Command = command;
			UI = pUI;
			id=0;
		}

		virtual bool executeFilter(const rtree::rect<f32>& bound)
		{
			return true;
		}

		virtual s32 getTargetLevel()
		{
			return -1;
		}

		virtual void visitNode(rtree::GRtNode* node,vector<rtree::GRtNode*>& path)
		{
			if(node->isLeaf())
			{
				rtree::GRtLeaf* pLeaf = static_cast<rtree::GRtLeaf*>(node);
				vector<rtree::GRtItem>& items = pLeaf->getItems();
				for(vector<rtree::GRtItem>::iterator it = items.begin();
					it!=items.end();it++)
				{
					rtree::GRtItem& item = *it;
					u32 id = item.Item->getId();

					Command->Parameters->Item[0L]->Value = _variant_t(item.IndexKey);	
					Command->Parameters->Item[1L]->Value = _variant_t(id);				
					Command->Execute(NULL,NULL,adCmdText|adExecuteNoRecords);	
					id++;
					if(UI)
					{
						UI->OnProgress(L"UPDATE_IK",id);
					}
				}
			}
		}
	};


	//!构建索引
	void GVectorDataLayerMDB::UpdateSpatialIndex(IProgressUI* pUI)
	{
		if(RTree)
		{	
			RTree->uninitialize();
			delete RTree;
		}

		RtNextId = 0;

		RTree = new rtree::GRTree();
		RTree->initialize(this,_RtMinChildCount,_RtMaxChildCount,_RtMinItemCount,_RtMaxItemCount);
				
		for(vector<GQRtItem*>::iterator it= QItems.begin();it!=QItems.end();it++)
		{
			delete (*it);
		}
		QItems.clear();

		wstring sql=
			wstring(L"select GM_ID,GM_MINX,GM_MINY,GM_MAXX,GM_MAXY,GM_QCODE from GM_")+Name;
		
		_CommandPtr command(__uuidof(Command));
		command->ActiveConnection =GetDataSourceMDB()->GetDBConnection();
		command->CommandText = _bstr_t(sql.c_str());

		_RecordsetPtr rs=command->Execute(NULL,NULL,adCmdText);
		
		int counter=0;
		
		GRectangle2dd box(1e10,1e10,-1e10,-1e10);
		if (!rs->BOF)
			rs->MoveFirst();
		while(!rs->AdoEOF)
		{
			u32 id   = rs->Fields->Item[0L]->Value;
			f64 minx = rs->Fields->Item[1L]->Value;	
			f64 miny = rs->Fields->Item[2L]->Value;
			f64 maxx = rs->Fields->Item[3L]->Value;
			f64 maxy = rs->Fields->Item[4L]->Value;
			
			box.AddRectangle(GRectangle2dd(minx,miny,maxx,maxy));
			GQRtItem* item = new GQRtItem(id,rect<f32>(minx,miny,maxx,maxy));

			_variant_t vArray = rs->Fields->Item[5L]->Value;
			memcpy(item->getQCODE(),vArray.parray->pvData,sizeof(QCODE));

	

			RTree->insertItem(item);
			QItems.push_back(item);

			rs->MoveNext();
			counter++;
			if(pUI)
			{
				pUI->OnProgress(L"GET_FEATURE_BOX",counter,counter);
			}
		}
		sql=
			wstring(L"update  GM_LAYER_REGISTER_TABLE set GM_MINX=?,GM_MINY=?,GM_MAXX=?,GM_MAXY=? where GM_NAME='")+Name+wstring(L"'");

		{
			_CommandPtr command(__uuidof(Command));
			command->ActiveConnection = GetDataSourceMDB()->GetDBConnection();
			command->CommandText = _bstr_t(sql.c_str());

			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MINX"),GFieldDef::ftDouble,8,_variant_t(box.MinEdge.X));
			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MINY"),GFieldDef::ftDouble,8,_variant_t(box.MinEdge.Y));
			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MAXX"),GFieldDef::ftDouble,8,_variant_t(box.MaxEdge.X));
			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_MAXY"),GFieldDef::ftDouble,8,_variant_t(box.MaxEdge.Y));
			//GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_NAME"),GFieldDef::ftVarWChar,8,_variant_t(_bstr_t(Name.c_str())));

			BoundingBox = box;
			command->Execute(NULL,NULL,adCmdText|adExecuteNoRecords);
		}	

		RTree->BuildIndexKey();	

		if(pUI)
		{
			pUI->OnSetTotal(L"UPDATE_IK",counter);
		}

		{
			sql =wstring(L"update GM_")+Name
				+wstring(L" set GM_IK=? where GM_ID=?");

			_CommandPtr command(__uuidof(Command));
			command->ActiveConnection = GetDataSourceMDB()->GetDBConnection();
			command->CommandText = _bstr_t(sql.c_str());
			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_IK"),GFieldDef::ftInteger,4);
			GSqlHelper::AddSqlCommandParamter(command,wstring(L"GM_ID"),GFieldDef::ftInteger,4);
						
			GmIKWriter	ikWriter(command,pUI);
			RTree->visitDepthFirst(&ikWriter);
		}
	}
	
	//!是否有空间索引
	GVectorDataLayer::EnumSpatialIndexType GVectorDataLayerMDB::GetSpatialIndexType()
	{
		if(RTree)
		{
			return GVectorDataLayer::SIT_RTREE;
		}
		else
		{
			return GVectorDataLayer::SIT_NONE;
		}
	}

	//!设置坐标系
	void	GVectorDataLayerMDB::SetCoordinateSystem(GCoordinateSystem* pCS)
	{
		__super::SetCoordinateSystem(pCS);
		string cs("unknown");
		if(pCS)
		{
			cs = pCS->GetName();
		}


	}

	//!创建新的数据查询
	GVectorDataQuery* GVectorDataLayerMDB::CreateQueryCommand(bool bGeometry)
	{
		GVectorDataQueryMDB* pQuery = new GVectorDataQueryMDB(this,bGeometry);
		return pQuery;
	}

	//!创建数据制图查询
	GVectorDataQuery* GVectorDataLayerMDB::CreateMapDrawingQueryCommand()
	{
		return new GVectorDataQueryMDB(this,true); 
	}


	//!得到数据插入
	GVectorDataInsert* GVectorDataLayerMDB::CreateInsertCommand()
	{
		return new GVectorDataInsertMDB(this);
	}

	
	//!得到RTree
	rtree::GRTree*		GVectorDataLayerMDB::GetRTree()
	{
		return RTree;
	}

	//////////////////////////////////////////////////////////////
	//				IRTreeSite
	//////////////////////////////////////////////////////////////
	rtree::GRtNode* GVectorDataLayerMDB::loadNode(rtree::GRTree* owner,u32 id)
	{
		return NULL;
	}

	void GVectorDataLayerMDB::save()
	{
	
	}

	void GVectorDataLayerMDB::deleteNode(rtree::GRtNode* node)
	{
	
	}

	void GVectorDataLayerMDB::addNode(rtree::GRtNode* node)
	{
		
	}

	bool GVectorDataLayerMDB::isInitialized()
	{
		return true;
	}

	u32 GVectorDataLayerMDB::nextId()
	{
		RtNextId++;
		return RtNextId;		 
	}

	void GVectorDataLayerMDB::getInitialParams(u32& rootId,u32& minChildCount,u32& maxChildCount,u32& minItemCount,u32& maxItemCount)
	{
		
	}

	void GVectorDataLayerMDB::initialize(u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount)
	{
		
	}

}
