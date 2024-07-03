#include "GRtLeaf.h"
#include "GRtIndex.h"
#include "GRTree.h"
#include "GRtHelper.h"
#include <algorithm>

namespace gmap{namespace rtree{

	//!���캯��
	GRtLeaf::GRtLeaf(GRTree* owner, GRtIndex* parent,u32 id,const rect<f32>& bound,const vector<GRtItem>& items)
		:GRtNode(owner,id)
	{
		Bound = bound;

		u32 size = items.size();
		for(u32 i=0;i<size;i++)
		{
			Items.push_back(items[i]);
			items[i].Item->setOwner(this,i);
		}
	}

	//!��������
	GRtLeaf::~GRtLeaf()
	{

	}

	//!��ȡ��Χ��
	const rect<f32>& 	GRtLeaf::getBound()
	{
		return Bound;
	}

	//!�Ƿ�Ҷ�ӽڵ�
	bool GRtLeaf::isLeaf()
	{
		return true;
	}

	//!�Ƿ���ڵ�
	bool GRtLeaf::isRoot()
	{
		return false;
	}


	//!�ж��ܷ�ж��
	bool GRtLeaf::canUnload()
	{
		return false;
	}

	//!������Ŀ
	void GRtLeaf::insertItem(IRtItem* pItem)
	{
		GRtItem item;
		item.Item = pItem;
		rect<f32> bound = pItem->getBound();
		Items.push_back(item);
		setDirty();

		if(Items.size()>Owner->getMaxItemCount())
		{
			rect<f32>  boundA,boundB;
			vector<u32> groupA,groupB;
			splitBounds<GRtItem>(Items,boundA,groupA,boundB,groupB,Owner->getMinItemCount());

			u32 size;

			size = groupA.size();
			vector<GRtItem> itemsA;
			for(u32 i=0;i<size;i++)
			{
				itemsA.push_back(Items[groupA[i]]);
			}

			size = groupB.size();
			vector<GRtItem> itemsB;
			for(u32 i=0;i<size;i++)
			{
				itemsB.push_back(Items[groupB[i]]);
			}

			Bound = boundA;
			Items = itemsA;
			int n=0;
			for(vector<GRtItem>::iterator it = Items.begin();
				it!=Items.end();it++)
			{
				it->Item->setOwner(this,n++);
			}
			Parent->adjustTree(this);

			GRtLeaf* newLeaf = new GRtLeaf(Owner,Parent,Owner->nextId(),boundB,itemsB);
			Parent->addChildNode(newLeaf);	
			Owner->getCache()->addNode(newLeaf);	
			newLeaf->Release();
		}
		else
		{
			Bound = combineBounds<f32>(Bound,bound);
			Parent->adjustTree(this);
		}
	}

	//!ɾ����Ŀ
	void GRtLeaf::deleteItem(IRtItem* pItem)
	{
		u32 index = pItem->getOwnerIndex();
		if(index==0xFFFFFFFF)
		{
			u32 size = Items.size();
			for(u32 i=0;i<size;i++)
			{
				if(Items[i].Item == pItem)
				{
					index= i;
					break;
				}
			}
		}
		if(index!=0xFFFFFFFF)
		{
			Items.erase(Items.begin()+pItem->getOwnerIndex());
			pItem->setOwner(NULL,0xFFFFFFFF);
		}
		else
		{
			return;
		}


		vector<GRtIndex*> toReinsetNodes;

		if(Items.size()==0)
		{
			Parent->confenseTree(toReinsetNodes,this);		
			Owner->getCache()->deleteNode(this);	
			this->Release();
		}
		else if(Items.size()<Owner->getMinItemCount() && Parent->getChildrenCount()>1 )
		{	
			vector<GRtNode*> path;
			Parent->confenseTree(toReinsetNodes,this);
			u32 treeHeight = Owner->getDepth();	

			for(s32 i  = toReinsetNodes.size()-1;i>-1;i--)
			{
				GRtIndex* reinsertNode = toReinsetNodes[i];
				u32 level = treeHeight- i-1;
				u32 nChildCount = reinsertNode->getChildrenCount();

				for(u32 n =0;n<nChildCount;n++)
				{
					GRtIndex* childNode = static_cast<GRtIndex*>(reinsertNode->getChildNode(n));
					const rect<f32>& bound = childNode->getBound();
					path.clear();
					GRtIndex* parentNode = Owner->chooseIndex(bound,path,level);
					parentNode->addChildNode(childNode);
				}		
			}

			for(u32 i=0;i<Items.size();i++)
			{
				GRtItem& item = Items[i];
				vector<GRtNode*> path;
				GRtLeaf* leaf = Owner->chooseLeaf(item.getBound(),path);
				leaf->insertItem(item.Item);
			}	

			for(u32 i  =0;i< toReinsetNodes.size();i++)
			{
				GRtIndex* reinsertNode = toReinsetNodes[i];
				Owner->getCache()->deleteNode(reinsertNode);
				reinsertNode->Release();
			}
			Owner->getCache()->deleteNode(this);
			this->Release();
		}
		else
		{
			Bound = combineBounds<GRtItem>(Items);
			Parent->adjustTree(this);
		}

	}

	void GRtLeaf::sort()
	{
		for(int i=0;i<Items.size();i++)
		{
			GRtItem& it = Items[i];

			it.IndexKey = this->getOwner()->GetHbCode(it.getBound().getCenter().X,it.getBound().getCenter().Y);
		}

		std::sort(Items.begin(),Items.end());
	}



	//!��ȡ���������е���Ŀ
	vector<GRtItem>& GRtLeaf::getItems()
	{
		return Items;
	}

}}