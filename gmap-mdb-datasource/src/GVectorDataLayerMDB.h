#ifndef __G_VECTOR_DATA_LAYER_MDB_H__
#define __G_VECTOR_DATA_LAYER_MDB_H__

#include "GCoreBase.h"
#include "adoInclude.h"
#include "GVectorDataLayer.h"
#include "GRTree.h"
#include "GRtDefs.h"
#include "GQindex.h"

using namespace gmap::rtree;


namespace gmap{

	class GVectorDataQueryMDB;
	class GVectorDataSourceMDB;
	class GVectorDataLayerDef;

	//!MDB图层
	class GVectorDataLayerMDB:public GVectorDataLayer,public rtree::IRTreeSite
	{
	public:
		GVectorDataLayerMDB(GVectorDataSourceMDB* pDataSource,const wstring& name,const GRectangle2dd& bounds,GVectorDataLayerDef* pLayerDef);

		~GVectorDataLayerMDB(void);

		virtual GVectorDataSource* GetDataSource();

		//!图层类型
		virtual EnumVectorLayerType GetLayerType();

		//!构建索引
		virtual void UpdateSpatialIndex(IProgressUI* pUI);

		//!是否有空间索引
		EnumSpatialIndexType		 GetSpatialIndexType();

		//!得到包围盒
		virtual const GRectangle2dd& GetBoundingBox();

		//!创建新的数据查询
		virtual GVectorDataQuery*   CreateQueryCommand(bool bGeometry);

		//!创建数据制图查询
		virtual GVectorDataQuery*   CreateMapDrawingQueryCommand();

		//!得到数据插入
		virtual GVectorDataInsert*  CreateInsertCommand();

		//!设置坐标系
		virtual void				SetCoordinateSystem(GCoordinateSystem* pCS);
	public:
		//////////////////////////////////////////////////////////////
		//				IRTreeSite
		//////////////////////////////////////////////////////////////
		virtual rtree::GRtNode* loadNode(rtree::GRTree* owner,u32 id);

		virtual void save();

		virtual void deleteNode(rtree::GRtNode* node);

		virtual void addNode(rtree::GRtNode* node);

		virtual bool isInitialized();

		virtual u32 nextId();

		virtual void getInitialParams(u32& rootId,u32& minChildCount,u32& maxChildCount,u32& minItemCount,u32& maxItemCount);

		virtual void initialize(u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount);

	private:
		//得到表名
		wstring GetSqlTableName();


		virtual GVectorDataSourceMDB* GetDataSourceMDB();

		//!创建SQL命令对象
		_CommandPtr	CreateSqlCommand();

		//!得到RTree
		rtree::GRTree*		GetRTree();


		friend class GVectorDataInsertMDB;
		friend class GVectorDataQueryMDB;

	public:
		class GQRtItem:public IRtItem
		{
		public:
			GQRtItem(u32 id,const rect<f32>& bound)
			{
				mId = id;
				mBound = bound;
			}
			virtual u32			getId(){return mId;}
			virtual rect<f32>	getBound(){return mBound;}
			virtual void		setOwner(GRtLeaf* leaf,u32 index){mOwner = leaf;mOwnerIndex = index;}
			virtual GRtLeaf*	getOwner(){return mOwner;}
			virtual u32			getOwnerIndex(){return mOwnerIndex;}
			QCODE*				getQCODE(){return &mQCODE;}
		private:
			u32					mId;
			rect<f32>			mBound;
			GRtLeaf*			mOwner;
			u16					mOwnerIndex;
			QCODE				mQCODE;
		};

	private:
		vector<GVectorDataQueryMDB*> MapDrawingQueryList;
		GVectorDataSourceMDB*		 DataSource;
		GRectangle2dd				 BoundingBox;
		GVectorDataQueryMDB*		 DefaultMapQuery;
		rtree::GRTree*				 RTree;
		vector<GQRtItem*>			 QItems;


		//////////////////////////////////////////////////
		u32							RtMinChildCount;
		u32							RtMaxChildCount;
		u32							RtMinItemCount;
		u32							RtMaxItemCount;
		u32							RtNextId;
	};

}


#endif
