#include "GRTree.h"
#include "GRtNode.h"
#include "GRtIndex.h"
#include "GRtLeaf.h"
#include "HilbertCurve.h"
#include "GRtVsitor.h"

namespace gmap{namespace rtree{

	//!构造函数
	GRTree::GRTree()
	{
		RootId = 0xFFFFFFFF;
		Root = NULL;
		Cache = NULL;

		MinItemCount=0;
		MaxItemCount=0;

		MinChildCount=0;
		MaxChildCount=0;
	}

	//!析构函数
	GRTree::~GRTree()
	{

	}

	//!初始化
	void GRTree::initialize(IRTreeSite* cache)
	{
		Cache = cache;
		Cache->getInitialParams(RootId,MinChildCount,MaxChildCount,MinItemCount,MaxItemCount);
	}

	//!初始化
	void GRTree::initialize(IRTreeSite* cache,u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount)
	{
		Cache = cache;

		MinChildCount = minChildCount;MaxChildCount = maxChildCount;
		MinItemCount = minItemCount;MaxItemCount = maxItemCount;
		Cache->initialize(MinChildCount,MaxChildCount,MinItemCount,MaxItemCount);

		RootId = 0xFFFFFFFF;
		Root = NULL;

	}

	//!初始化
	void GRTree::uninitialize()
	{
		Cache->save();
		if(Root)
		{
			Root->Release();
		}

		RootId = 0xFFFFFFFF;
		Root = NULL;
		Cache = NULL;

		MinItemCount=0;
		MaxItemCount=0;

		MinChildCount=0;
		MaxChildCount=0;
	}

	//!保存所有数据
	void GRTree::save()
	{
		Cache->save();	
	}

	//!获取Cache
	IRTreeSite* GRTree::getCache()
	{
		return Cache;
	}

	//!获取下一个Id
	u32 GRTree::nextId()
	{
		return Cache->nextId();
	}

	//!获取子节点数
	u32 GRTree::getMaxChildCount()
	{
		return MaxChildCount;
	}

	//!获取最小子节点数量
	u32 GRTree::getMinChildCount()
	{
		return MinChildCount;
	}

	//!获取最大的项目数
	u32 GRTree::getMaxItemCount()
	{
		return MaxItemCount;
	}

	//!获取最小的项目数
	u32 GRTree::getMinItemCount()
	{
		return MinItemCount;
	}

	//!获取根节点
	GRtIndex* GRTree::getRoot()
	{
		if(Root == NULL && RootId != 0xFFFFFFFF)
		{
			Root =static_cast<GRtIndex*>( Cache->loadNode(this,RootId));
			if(Root!=NULL)
			{
				Root->AddRef();
			}
		}
		return Root;
	}

	//!遍历
	void GRTree::visit(IRtVisitor* visitor)
	{
		if(getRoot())
		{
			vector<GRtNode*> path;
			//IseRtFilter* filter = visitor->getFilter();
			if(visitor->executeFilter(getRoot()->getBound()))
			{
				path.push_back(getRoot());

				Root->visit(visitor,path);

				if(  visitor->getTargetLevel()==0 
					||  visitor->getTargetLevel() == -2)
				{
					visitor->visitNode(getRoot(),path);
				}
			}
		}
	}


	//!遍历
	void GRTree::visitDepthFirst(IRtVisitor* visitor)
	{	
		vector<GRtNode*> path;
		if(getRoot())
		{
			if(visitor->executeFilter(getRoot()->getBound()))
			{
				Root->visitDepthFirst(visitor,path);
			}
		}
	}


	u32 GRTree::getDepth()
	{
		if(getRoot())
		{
			return getRoot()->getDepth();
		}
		else
		{
			return -1;
		}
	}
	//!插入数据项
	void GRTree::insertItem(IRtItem* item)
	{
		if(getRoot())
		{		
			vector<GRtNode*> path;
			GRtLeaf* leaf = getRoot()->chooseLeaf(item->getBound(),path);
			leaf->insertItem(item);		
		}
		else
		{
			vector<GRtItem> items;
			GRtItem newItem;
			newItem.Item=item;
			rect<f32> bound = item->getBound();
			items.push_back(newItem);
			GRtLeaf *leaf = new GRtLeaf(this,NULL,nextId(),bound,items);


			vector<GRtEntry> entries;
			GRtEntry entry;
			entry.Id =  leaf->getId();
			entry.Bound  = leaf->getBound();
			entry.Node = leaf;
			entries.push_back(entry);

			GRtIndex* root = new GRtIndex(this,NULL,nextId(),bound,entries);
			leaf->Release();

			Root = root;
			RootId = Root->getId();

			getCache()->addNode(leaf);
			getCache()->addNode(root);
		}
	}

	//!删除数据项
	void GRTree::deleteItem(IRtItem* item)
	{
		GRtLeaf* leaf = item->getOwner();

		//GRtLeaf* leaf = searchLeaf(id,bound);
		if(leaf!=NULL)
		{
			//u32 index = leaf->IndexOf(id);
			leaf->deleteItem(item);
		}

	}
	//!选择叶子节点
	GRtLeaf*	GRTree::chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path)
	{
		if(getRoot()!=NULL)
		{
			return getRoot()->chooseLeaf(bound,path);
		}
		return NULL;
	}

	//!选择索引节点
	GRtIndex*  GRTree::chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter)
	{
		if(getRoot()!=NULL)
		{
			if(levelCounter==0)
			{
				return getRoot();
			}
			else
			{
				return getRoot()->chooseIndex(bound,path,levelCounter);
			}
		}
		return NULL;
	}

	//!查找叶子节点
	GRtLeaf*  GRTree::searchLeaf(u32 id,const rect<f32>& bound)
	{
		if(getRoot()!=NULL)
		{
			return getRoot()->searchLeaf(id,bound);
		}
		return NULL;
	}


	void GRTree::switchRoot(GRtIndex* root)
	{
		GRtIndex* oldRoot = Root;

		Root = root;
		if(Root)
		{
			Root->setParent(NULL);
			Root->setIndex(0);		
			Root->AddRef();
		}
		else
		{

		}

		if(oldRoot != NULL)
		{
			oldRoot->Release();
		}
	}

	//!进行希尔伯特排序
	void GRTree::BuildIndexKey()
	{
		rect<f32> bound = Root->getBound();

		f32 w = bound.getWidth();
		f32 h = bound.getHeight();

		f32 size = w>h?w:h;

		HbBound = rect<f64>(bound.UpperLeftCorner.X,bound.UpperLeftCorner.Y,
			bound.UpperLeftCorner.X+size,bound.UpperLeftCorner.Y+size);

		const int LevelPlus = 4;
		HbLevelCount = LevelPlus+this->getDepth();

		HbCellSize = size / (0x1<<HbLevelCount);


		GRtHbKeySettor hbKeySettor;
		visitDepthFirst(&hbKeySettor);

		GRtItemIndexKeySettor indexKeySettor;
		visitDepthFirst(&indexKeySettor);



	}

	struct HbPoint
	{
		typedef u32 IndexType;

		HbPoint(u32 ix,u32 iy)
		{
			X = ix;
			Y = iy;
		}

		u32 X;
		u32 Y;
	};

	//!得到排序码
	u32 GRTree::GetHbCode(f64 x,f64 y)
	{
		u32 ix  = (x-HbBound.UpperLeftCorner.X)/HbCellSize;
		u32 iy =  (y-HbBound.UpperLeftCorner.Y)/HbCellSize;

		return HilbertCurve::HbCodeFromPoint<HbPoint,HbPoint::IndexType>(
			HbPoint(ix,iy),HbLevelCount,1);
	}

}}