#include "GVectorDataQueryMDB.h"
#include "GVectorDataLayerMDB.h"
#include "GVectorDataSourceMDB.h"
#include "GFieldDef.h"
#include "GRtLeaf.h"
#include "GRange.h"
#include "GSqlHelper.h"
#include <algorithm>

namespace gmap{

	class GVectorDataQueryResultMDB:public GVectorDataQueryResult
	{
	public:
		GVectorDataQueryResultMDB(GVectorDataQueryMDB* pQuery,_RecordsetPtr rs,int geomField)
		{
			Query = pQuery;
			DbRecordset = rs;
			GeomField = geomField;
			Dirty =true;
			Geometry = NULL;

			int count = DbRecordset->Fields->Count-1-(GeomField!=-1?1:0);

			for(int i=0;i<count;i++)
			{
				_bstr_t name = DbRecordset->Fields->Item[long(1+i)]->Name;
				int type =  DbRecordset->Fields->Item[long(1+i)]->Type;
				int size = DbRecordset->Fields->Item[long(1+i)]->DefinedSize;

				GFieldDef* pDef =  new GFieldDef(wstring((wchar_t*)name),(GFieldDef::EnumFieldType)type,size);
				FieldsDef.push_back(pDef);
				ValueCache.push_back(_variant_t());
			}
			Reset();
		}

		~GVectorDataQueryResultMDB()
		{
			for(int i=0;i<FieldsDef.size();i++)
			{
				FieldsDef[i]->Release();
			}
			if(Geometry)
			{
				Geometry->Release();
			}
		}

		//!重置
		virtual void Reset()
		{
			if(!DbRecordset->BOF)
				DbRecordset->MoveFirst();
			if (!IsEOF())
			{
				_update_cache_field_value();
			}
		}

		//!是否已经结束
		virtual bool IsEOF()
		{
			return DbRecordset->AdoEOF==VARIANT_TRUE;
		}

		//!移动
		virtual bool MoveNext()
		{
			DbRecordset->MoveNext();
			Dirty  =true;
			if (!IsEOF())
			{
				_update_cache_field_value();
			}
			return !IsEOF();
		}

		//!得到Id
		virtual u32 GetId()
		{
			return (u32)(DbRecordset->Fields->Item[0L]->Value);
		}

		//!得到几何
		virtual GGeometry* GetGeometry()
		{
			if(Dirty&&GeomField!=-1)
			{
				u32 size = DbRecordset->Fields->Item[long(GeomField)]->ActualSize;
				_variant_t vBuffer=DbRecordset->Fields->Item[(long)GeomField]->GetChunk(size);
				BYTE* pData=NULL;
				SafeArrayAccessData(vBuffer.parray,(void**)&pData);
				GMemReadFile* pReadFile = GMemReadFile::Create(pData,size);

				if(Geometry)Geometry->Release();
				Geometry = GGeometry::CreateFromStream(pReadFile);
				pReadFile->Release();
				SafeArrayUnaccessData(vBuffer.parray);
				Dirty=false;
			}
			return Geometry;
		}

		//!得到字段值
		virtual void GetFieldValue(u32 index,_variant_t& val)
		{
			//val = DbRecordset->Fields->Item[long(1+index)]->Value;
			val = ValueCache[index];
		}

		//!得到字段的值
		virtual void GetFieldValue(const wstring& fieldName,_variant_t& val)
		{
			//val = DbRecordset->Fields->Item[_bstr_t(fieldName.c_str())]->Value;
			for (int i = 0; i < FieldsDef.size(); i++)
			{
				if (FieldsDef[i]->Name == fieldName)
				{
					val = ValueCache[i];
					return;
				}
			}
		}

		//!得到字段个数
		virtual  u32 GetFieldCount()
		{
			return DbRecordset->Fields->Count-1-(GeomField!=-1?1:0);
		}

		//!得到字段定义
		virtual  GFieldDef* GetFieldDef(u32 index)
		{
			return FieldsDef.at(index);

		}
	private:
		void _update_cache_field_value()
		{
			ADODB::FieldsPtr fields = DbRecordset->Fields;
			for (int i = 0; i < FieldsDef.size(); i++)
			{
				ValueCache[i]=fields->Item[long(i + 1)]->Value;
			}
		}
	private:
		GVectorDataQueryMDB* Query;
		_RecordsetPtr		DbRecordset;
		bool				Dirty;
		int				    GeomField;
		vector<GFieldDef*>  FieldsDef;
		vector<_variant_t>  ValueCache;
		GGeometry*		    Geometry;

	};

	//------------------------------------------------------------------------------------------
	GVectorDataQueryMDB::GVectorDataQueryMDB(GVectorDataLayerMDB* pDataLayer,bool geomertyField)
		:GVectorDataQuery(geomertyField)
	{
		DataLayerMDB=pDataLayer;
	}

	GVectorDataQueryMDB::~GVectorDataQueryMDB(void)
	{

	}
	//------------------------------------------------------------------------------------
	class QueryVisitor:public rtree::IRtVisitor
	{
	public:
		vector<GRange> Ranges;
		const  rtree::rect<f32>& QueryBound;
	public:
		QueryVisitor(const rtree::rect<f32>& bound):QueryBound(bound)
		{
		}
		virtual bool executeFilter(const  rtree::rect<f32>& bound)
		{
			return bound.isRectCollided(QueryBound);//||QueryBound.isPointInside(bound.UpperLeftCorner)||QueryBound.isPointInside(bound.LowerRightCorner);
		}

		virtual s32 getTargetLevel()
		{
			return -1;
		}

		virtual void visitNode(rtree::GRtNode* node,vector<rtree::GRtNode*>& path)
		{
			rtree::GRtLeaf *pLeaf= static_cast<rtree::GRtLeaf*>(node);		

			const rtree::rect<f32>& bound = pLeaf->getBound();

			//!获取容器中所有的项目
			vector<rtree::GRtItem>& items= pLeaf->getItems();
			if(QueryBound.isPointInside(bound.UpperLeftCorner)&&QueryBound.isPointInside(bound.LowerRightCorner))
			{
				GRange range;
				range.a = items[0].IndexKey;
				range.b = items[items.size()-1].IndexKey;
				Ranges.push_back(range);
				return;
			}

			u32 sz = items.size();
			GRange range;
			range.a=range.b=0;
			for(u32 i=0;i<sz;i++)
			{
				rtree::GRtItem& item = items[i];
				if (item.Item->getBound().isRectCollided(QueryBound)||QueryBound.isRectCollided(item.Item->getBound()))
				{
					u32 b=item.IndexKey;
					if(range.a==0)
					{
						range.a = range.b =b;
					}
					if(b-range.b>3)
					{
						Ranges.push_back(range);
						range.a = range.b = b;
					}
					else
					{
						range.b = b;
					}
				}
			}

			if(range.a !=0)
			{
				Ranges.push_back(range);
			}
		}
	};

	inline void combineRanges(vector<GRange>& ranges)
	{
		std::sort(ranges.begin(),ranges.end());
		u32 it=0;
		static int MaxD = 10;
		static int MaxSize = 64;


		while(it<ranges.size()-1)
		{
			u32 d=ranges[it+1].a-ranges[it].b;
			if(d<MaxD)
			{
				ranges[it].b=ranges[it+1].b;
				ranges.erase(ranges.begin()+it+1);
			}
			else
			{
				it++;
			}
		}
		while(ranges.size()>MaxSize)
		{
			u32 size=ranges.size();
			u32 n=-1;
			u32 D=1000000;	
			for(u32 i=0;i<size-1;i++)
			{
				u32 d = ranges[i+1].a-ranges[i].b;
				if(d<D)
				{
					D=d;	
					n=i;
				}
			}
			if(n!=-1)
			{
				ranges[n].b=ranges[n+1].b;
				ranges.erase(ranges.begin()+n+1);
			}
		}
	}

	wstring getRangeCondition(const GRange& range)
	{
		wchar_t strA[16];
		wchar_t strB[16];
		_itow_s(range.a,strA,10);
		_itow_s(range.b,strB,10);
		if(range.a==range.b)
			return wstring(L" GM_IK=")+wstring(strA);
		else
			return wstring(L" GM_IK between ")+wstring(strA)+wstring(L" and ")+wstring(strB);
	}

	//------------------------------------------------------------------------------------

	GVectorDataQueryResult* GVectorDataQueryMDB::Execute()
	{
		_CommandPtr command(__uuidof(Command));
		command->ActiveConnection = DataLayerMDB->GetDataSourceMDB()->GetDBConnection();

		wstring sql = 
			wstring(L"select GM_ID");

		for(int i=0;i<QueryFieldList.size();i++)
		{
			sql+=wstring(L",")+QueryFieldList[i];
		}

		int geomField =-1;

		if(this->QueryGeometryField)
		{
			geomField=QueryFieldList.size()+1;
			sql+=wstring(L",GM_GEOMETRY ");
		}

		sql+=wstring(L" from GM_")+DataLayerMDB->GetName();
		if (SqlWhere.size()>0 || SpatialFilter != NULL)
		{
			sql += wstring(L" where ");
		}
		if (SqlWhere.size() > 0)
		{
			sql += SqlWhere;
		}
		if (SpatialFilter)
		{
			if(SqlWhere.size() > 0)
				sql += wstring(L" and ");

			GRectangle2dd box = SpatialFilter->GetBoundingBox();
			if (DataLayerMDB->GetRTree() == NULL)
			{
				sql += wstring(L"( GM_MAXX>? and GM_MAXY>? and GM_MINX<? and GM_MINY<? )");
				GSqlHelper::AddSqlCommandParamter(command, wstring(L"minx"), GFieldDef::ftDouble, 8, _variant_t(box.MinEdge.X));
				GSqlHelper::AddSqlCommandParamter(command, wstring(L"miny"), GFieldDef::ftDouble, 8, _variant_t(box.MinEdge.Y));
				GSqlHelper::AddSqlCommandParamter(command, wstring(L"maxx"), GFieldDef::ftDouble, 8, _variant_t(box.MaxEdge.X));
				GSqlHelper::AddSqlCommandParamter(command, wstring(L"maxy"), GFieldDef::ftDouble, 8, _variant_t(box.MaxEdge.Y));
			}
			else
			{
				rtree::rect<f32> rect(box.MinEdge.X, box.MinEdge.Y, box.MaxEdge.X, box.MaxEdge.Y);
				QueryVisitor visitor(rect);
				DataLayerMDB->GetRTree()->visitDepthFirst(&visitor);
				vector<GRange>& ranges = visitor.Ranges;
				if (ranges.size() > 0)
				{
					combineRanges(ranges);
					sql += getRangeCondition(ranges[0]);
					u32 count = ranges.size();
					for (u32 i = 1; i < count; i++)
					{
						sql += wstring(L" or ") + getRangeCondition(ranges[i]);
					}
				}
				else
				{
					sql += wstring(L" 1<0");
				}
			}
		}
		command->CommandText = _bstr_t(sql.c_str());
		_RecordsetPtr rs = command->Execute(NULL, NULL, adCmdText);
		return new GVectorDataQueryResultMDB(this, rs, geomField);
	}

	GVectorDataLayer* GVectorDataQueryMDB::GetDataLayer()
	{
		return this->DataLayerMDB;
	}


	class RtreeDrawer:public rtree::IRtVisitor
	{
	public:
		GMapDrawing*			  Drawing;
		rtree::rect<f32>          QueryBound;
		GQIndexRasterizer*		  Ras;	
		QCODE					  *QCode;
		vector<IRtItem*>		  RtItemsQ;
		vector<IRtItem*>		  RtItemsB;
		vector<IRtItem*>		  RtItemsQB;
		Pen						  BoxPen;

	public:
		RtreeDrawer(GQIndexRasterizer*	ras,GMapDrawing* pDrawing,
			const  rtree::rect<f32>& bound,	QCODE*	qcode):
		QueryBound(bound),BoxPen(Color::Red)
		{
			Drawing = pDrawing;
			Ras = ras;		
			QCode = qcode;
			BoxPen.SetColor(Color::Red);
		}
		~RtreeDrawer()
		{

		}
		virtual bool executeFilter(const  rtree::rect<f32>& bound)
		{
			return bound.isRectCollided(QueryBound) ;
		}

		virtual s32 getTargetLevel()
		{
			return -2;
		}
		
		virtual void visitNode(rtree::GRtNode* node,vector<rtree::GRtNode*>& path)
		{
			if(node->isLeaf())
			{
				rtree::GRtLeaf *pLeaf= static_cast<rtree::GRtLeaf*>(node);
				vector<rtree::GRtItem>& items = pLeaf->getItems();
				Graphics* g = Drawing->GetGraphics();

				u32 size = items.size();
				for(int i=0;i<size;i++)
				{
					rtree::rect<f32> rc = items[i].getBound();
					srand(items[i].getId());
					if(QCode)
					{
						QCODE* qcode = ((GVectorDataLayerMDB::GQRtItem*)(items[i].Item))->getQCODE();
						bool a,b;
						a=false;
						b=false;
						if(a=QueryBound.isRectCollided(rc))
						{	
							RtItemsB.push_back(items[i].Item);
														
							PointF pt0,pt1;
							Drawing->MapToView(rc.UpperLeftCorner.X,rc.UpperLeftCorner.Y,pt0.X,pt0.Y);
							Drawing->MapToView(rc.LowerRightCorner.X,rc.LowerRightCorner.Y,pt1.X,pt1.Y);

							g->DrawLine(&BoxPen,pt0.X,pt0.Y,pt0.X,pt1.Y);
							g->DrawLine(&BoxPen,pt0.X,pt1.Y,pt1.X,pt1.Y);
							g->DrawLine(&BoxPen,pt1.X,pt1.Y,pt1.X,pt0.Y);
							g->DrawLine(&BoxPen,pt1.X,pt0.Y,pt0.X,pt0.Y);

						}
						if(b=QCodeOp::Intersect(*qcode,*QCode))
						{
							RtItemsQ.push_back(items[i].Item);
						}
						if(a&b)
						{
							RtItemsQB.push_back(items[i].Item);
							DrawQCode(Ras,Drawing,*qcode,Color(100,255,0,0));
						}
					}
					else
					{
						if(QueryBound.isRectCollided(rc))
						{						

							PointF pt0,pt1;
							Drawing->MapToView(rc.UpperLeftCorner.X,rc.UpperLeftCorner.Y,pt0.X,pt0.Y);
							Drawing->MapToView(rc.LowerRightCorner.X,rc.LowerRightCorner.Y,pt1.X,pt1.Y);

							g->DrawLine(&BoxPen,pt0.X,pt0.Y,pt0.X,pt1.Y);
							g->DrawLine(&BoxPen,pt0.X,pt1.Y,pt1.X,pt1.Y);
							g->DrawLine(&BoxPen,pt1.X,pt1.Y,pt1.X,pt0.Y);
							g->DrawLine(&BoxPen,pt1.X,pt0.Y,pt0.X,pt0.Y);

							RtItemsB.push_back(items[i].Item);

							QCODE* qcode = ((GVectorDataLayerMDB::GQRtItem*)(items[i].Item))->getQCODE();
							//DrawQCode(Ras,Drawing,*qcode,Color(100,255,0,0));

						}

					}
				}
			}
		}

	};



	void GVectorDataQueryMDB::DrawDebug(GMapDrawing* pDrawing,EnumSpatialFilterType filterType,GGeometry* filter)
	{
		GRectangle2dd box = filter?filter->GetBoundingBox():SpatialFilter->GetBoundingBox() ;
		Pen pen(Color::Red);
		PointF pt0,pt1;
		pDrawing->MapToView(box.MinEdge.X,box.MinEdge.Y,pt0.X,pt0.Y);
		pDrawing->MapToView(box.MaxEdge.X,box.MaxEdge.Y,pt1.X,pt1.Y);

		if(filterType==EnumSpatialFilterType::SFT_BOUNDING_BOX
		   ||
		   filter ==NULL
			)
		{
			Graphics* g = pDrawing->GetGraphics();

			g->DrawLine(&pen,pt0.X,pt0.Y,pt0.X,pt1.Y);
			g->DrawLine(&pen,pt0.X,pt1.Y,pt1.X,pt1.Y);
			g->DrawLine(&pen,pt1.X,pt1.Y,pt1.X,pt0.Y);
			g->DrawLine(&pen,pt1.X,pt0.Y,pt0.X,pt0.Y);

			if(DataLayerMDB->GetRTree()!=NULL)
			{
				RtreeDrawer drawer(DataLayerMDB->GetDataSourceMDB()->QCodeRasterizer, pDrawing,rtree::rect<f32>(box.MinEdge.X,box.MinEdge.Y,box.MaxEdge.X,box.MaxEdge.Y),NULL);
				DataLayerMDB->GetRTree()->visitDepthFirst(&drawer);

			}
		}
		else
		{
			GQIndexRasterizer* ras = DataLayerMDB->GetDataSourceMDB()->QCodeRasterizer;
			QCODE qcode = ras->RasterizeGeometry(filter);
			
			if(DataLayerMDB->GetRTree()!=NULL)
			{
				RtreeDrawer drawer(DataLayerMDB->GetDataSourceMDB()->QCodeRasterizer, pDrawing,rtree::rect<f32>(box.MinEdge.X,box.MinEdge.Y,box.MaxEdge.X,box.MaxEdge.Y),&qcode);
				DataLayerMDB->GetRTree()->visitDepthFirst(&drawer);
				
				if(	a!=drawer.RtItemsQ.size()&&
					b!=drawer.RtItemsB.size()&&
					c!=drawer.RtItemsQB.size())
				{
					//USES_CONVERSION;
					//TRACE1("%s,",DataLayerMDB->GetName().c_str());
					//TRACE3("q=%d,b=%d,qb=%d",drawer.RtItemsQ.size(),drawer.RtItemsB.size(),drawer.RtItemsQB.size());
				}

				a=drawer.RtItemsQ.size();
				b=drawer.RtItemsB.size();
				c=drawer.RtItemsQB.size();
			}

			DrawQCode(ras,pDrawing,qcode);
		}

	}
}
