#include "GRTree.h"
#include "GRtNode.h"
#include "GRtIndex.h"
#include "GRtLeaf.h"
#include "HilbertCurve.h"
#include "GRtVsitor.h"

namespace gmap{namespace rtree{

	//!���캯��
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

	//!��������
	GRTree::~GRTree()
	{

	}

	//!��ʼ��
	void GRTree::initialize(IRTreeSite* cache)
	{
		Cache = cache;
		Cache->getInitialParams(RootId,MinChildCount,MaxChildCount,MinItemCount,MaxItemCount);
	}

	//!��ʼ��
	void GRTree::initialize(IRTreeSite* cache,u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount)
	{
		Cache = cache;

		MinChildCount = minChildCount;MaxChildCount = maxChildCount;
		MinItemCount = minItemCount;MaxItemCount = maxItemCount;
		Cache->initialize(MinChildCount,MaxChildCount,MinItemCount,MaxItemCount);

		RootId = 0xFFFFFFFF;
		Root = NULL;

	}

	//!��ʼ��
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

	//!������������
	void GRTree::save()
	{
		Cache->save();	
	}

	//!��ȡCache
	IRTreeSite* GRTree::getCache()
	{
		return Cache;
	}

	//!��ȡ��һ��Id
	u32 GRTree::nextId()
	{
		return Cache->nextId();
	}

	//!��ȡ�ӽڵ���
	u32 GRTree::getMaxChildCount()
	{
		return MaxChildCount;
	}

	//!��ȡ��С�ӽڵ�����
	u32 GRTree::getMinChildCount()
	{
		return MinChildCount;
	}

	//!��ȡ������Ŀ��
	u32 GRTree::getMaxItemCount()
	{
		return MaxItemCount;
	}

	//!��ȡ��С����Ŀ��
	u32 GRTree::getMinItemCount()
	{
		return MinItemCount;
	}

	//!��ȡ���ڵ�
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

	//!����
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


	//!����
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
	//!����������
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

	//!ɾ��������
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
	//!ѡ��Ҷ�ӽڵ�
	GRtLeaf*	GRTree::chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path)
	{
		if(getRoot()!=NULL)
		{
			return getRoot()->chooseLeaf(bound,path);
		}
		return NULL;
	}

	//!ѡ�������ڵ�
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

	//!����Ҷ�ӽڵ�
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

	//!����ϣ����������
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

	//!�õ�������
	u32 GRTree::GetHbCode(f64 x,f64 y)
	{
		u32 ix  = (x-HbBound.UpperLeftCorner.X)/HbCellSize;
		u32 iy =  (y-HbBound.UpperLeftCorner.Y)/HbCellSize;

		return HilbertCurve::HbCodeFromPoint<HbPoint,HbPoint::IndexType>(
			HbPoint(ix,iy),HbLevelCount,1);
	}

}}