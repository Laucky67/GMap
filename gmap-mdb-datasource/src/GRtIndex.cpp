#include "GRtIndex.h"
#include "GRtLeaf.h"
#include "GRTree.h"
#include "GRtHelper.h"
#include <algorithm>

namespace gmap{namespace rtree
{
//!constructor
GRtIndex::GRtIndex(GRTree* owner,GRtIndex* parent,u32 id,const rect<f32>& bound,const vector<GRtEntry>& children)
	:GRtNode(owner,id)
{
	for(u32 i = 0;i<children.size();i++)
	{
		Children.push_back(children[i]);
		if(children[i].Node!=NULL)
		{
			children[i].Node->setParent(this);
			children[i].Node->setIndex(i);
			children[i].Node->AddRef();
		}
	}
	Parent = parent;
	Bound =  bound;
}

//!deconstructor
GRtIndex::~GRtIndex()
{
	for(u32 i = 0;i<Children.size();i++)
	{
		GRtEntry& item = Children[i];
		if(item.Node!=NULL)
		{
			item.Node->Release();
		}
	}
}

const rect<f32>& GRtIndex::getBound()
{
	return Bound;
}

//!是否叶子节点
bool GRtIndex::isLeaf()
{
	return false;
}

//!是否根节点
bool GRtIndex::isRoot()
{
	return Parent==NULL;
}


//!遍历树
void GRtIndex::visit(IRtVisitor* visitor,vector<GRtNode*>& path)
{
	u32 size = Children.size();
	//IseRtFilter* filter = visitor->getFilter();
	
	path.push_back(this);

	for(u32 i=0;i<size;i++)
	{
		if(visitor->executeFilter(Children[i].Bound))
		{
			GRtNode* node = getChildNode(i);	


			if(visitor->getTargetLevel()<0 || visitor->getTargetLevel()>= path.size())
			{
				if(!node->isLeaf())
				{
					(static_cast<GRtIndex*>(node))->visit(visitor,path);
				}
			}		
			
			if(path.size() == visitor->getTargetLevel() 
			|| (visitor->getTargetLevel() == -1 && node->isLeaf())
			||  visitor->getTargetLevel() == -2)
			{
				visitor->visitNode(node,path);
			}
		}
	}
	//path.set_used(path.size()-1);
}


//!遍历
void GRtIndex::visitDepthFirst(IRtVisitor* visitor,vector<GRtNode*>& path)
{
	u32 size = Children.size();
	path.push_back(this);

	for(u32 i=0;i<size;i++)
	{
		GRtNode* node = getChildNode(i);
		if(visitor->executeFilter(node->getBound()))
		{
			

			if(visitor->getTargetLevel()<0 || visitor->getTargetLevel()>= path.size())
			{
				if(!node->isLeaf())
				{
					(static_cast<GRtIndex*>(node))->visitDepthFirst(visitor,path);
				}
				else
				{
					visitor->visitNode(node,path);
				}
			}	
		}
	}

	if(path.size() == visitor->getTargetLevel() 
		|| (visitor->getTargetLevel() ==-2))
	{
		visitor->visitNode(this,path);
	}
	//path.set_used(path.size()-1);
}

//!判断能否卸载
bool GRtIndex::canUnload()
{
	if(ReferenceCounter >1 )
		return false;
	
	u32 size = Children.size();
    u32 counter =0;

	for(u32 i =0;i <size ;i++)
	{
		if(Children[i].Node!=NULL)
		{
			counter++;
		}
	}
	return counter == 0;
}

//!释放入口的节点对象
void GRtIndex::unloadChild (u32 index)
{
	Children[index].Node = NULL;
}

//!获取孩子的个数
u32 GRtIndex::getChildrenCount()
{
	return Children.size();	
}

//!获取孩子的Id
u32 GRtIndex::getChildId(u32 index)
{
	return Children[index].Id;
}

//!获取孩子节点
GRtNode* GRtIndex::getChildNode(u32 index)
{
	GRtNode* node = Children[index].Node;
	//if(node == NULL)
	//{
	//	node = Owner->getCache()->loadNode(Owner,Children[index].Id);
	//	node->setParent(this);
	//	node->setIndex(index);
	//	node->AddRef();
	//	node->setDirty();

	//	Children[index].Node = node;
	//}
	return node;	
}

//!获取孩子的范围
const rect<f32>& GRtIndex::getChildBound(u32 index)
{
	return Children[index].Bound;
}


//!选择叶子节点
GRtLeaf*	GRtIndex::chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path)
{
	
	u32 index = -1;
	f32 minValue = 1e+20f;

	u32 size = Children.size();

	for(u32 i =0;i<size;i++)
	{
		
		GRtNode* node  = getChildNode(i);
		u32 nChild = 0;
		if(node->isLeaf())
		{
			nChild=static_cast<GRtLeaf*>(node)->getItems().size();
		}
		else
		{
			nChild=static_cast<GRtIndex*>(node)->getChildrenCount();
		}	

		f32 retValue = computeDeltaValue(Children[i].Bound,bound,nChild);
		if(retValue< minValue)
		{
			index = i;
			minValue = retValue;
		}
	}

	path.push_back(this);
	GRtNode* node  = getChildNode(index);
	if(node->isLeaf())
	{
		return static_cast<GRtLeaf*>(node);
	}
	else
	{
		return (static_cast<GRtIndex*>(node))->chooseLeaf(bound,path);
	}	
}

//!选择索引节点
GRtIndex*  GRtIndex::chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter)
{
	u32 index = -1;
	f32 minValue = 1e+10f;
	u32 size = Children.size();

	for(u32 i =0;i<size;i++)
	{
		f32 retValue = computeDeltaValue(Children[i].Bound,bound);
		if(retValue< minValue)
		{
			index = i;
			minValue=retValue;
		}
	}

	path.push_back(this);
	levelCounter--;
	GRtNode* node  = getChildNode(index);

	if(levelCounter ==0 )
	{
		if(node->isLeaf())
		{
			return NULL;
		}
		else
		{
			return static_cast<GRtIndex*>(node);
		}
	}
	else
	{
		return (static_cast<GRtIndex*>(node))->chooseIndex(bound,path,levelCounter);	
	}
}


void GRtIndex::sort()
{	
	
	for(int i=0;i<Children.size();i++)
	{
		GRtEntry& et = Children[i];		
		et.IndexKey = this->getOwner()->GetHbCode(et.Bound.getCenter().X,et.Bound.getCenter().Y);
	}

	std::sort(Children.begin(),Children.end());
}


//!查找叶子节点
GRtLeaf*  GRtIndex::searchLeaf(u32 id,const rect<f32>& bound)
{
	u32 size = Children.size();

	for(u32 i =0;i<size;i++)
	{
		if(contains(Children[i].Bound,bound))
		{
			GRtNode* node= getChildNode(i);
			if(node->isLeaf())
			{
				GRtLeaf* leaf = static_cast<GRtLeaf*>(node);
				//if(leaf->IndexOf(id)!=-1)
				{
					return leaf;
				}
			}
			else
			{
				GRtIndex* indexNode = static_cast<GRtIndex*>(node);
				GRtLeaf* leaf = indexNode->searchLeaf(id,bound);
				if(leaf!=NULL)
				{
					return leaf;
				}
			}
		}
	}

	return NULL;
}

u32 GRtIndex::getHeight()
{
	if(Parent==NULL)
	{
		return 0;
	}
	else
	{
		return Parent->getHeight()+1;
	}		
}

u32 GRtIndex::getDepth()
{
	GRtNode* node = getChildNode(0);

	if(node->isLeaf())
	{
		return 1;
	}
	else
	{
		return ((static_cast<GRtIndex*>(node))->getDepth())+1;
	}
}

//!添加新节点
void GRtIndex::addChildNode(GRtNode* node)
{
	GRtEntry entry;
	entry.Id = node->getId();
	entry.Bound = node->getBound();
	entry.Node  = node;
	node->AddRef();

	Children.push_back(entry);
	node->setParent(this);
	node->setIndex(Children.size()-1);


	if(Children.size()>Owner->getMaxChildCount())
	{
		rect<f32> boundA,boundB;
		vector<u32> groupA,groupB;
		splitBounds<GRtEntry>(Children,boundA,groupA,boundB,groupB,Owner->getMinItemCount());
		
		u32 size;

		size = groupA.size();
		vector<GRtEntry> entriesA;
		for(u32 i=0;i<size;i++)
		{
			GRtEntry& entry = Children[groupA[i]];
			if(entry.Node!=NULL)
			{
				entry.Node->setIndex(i);
			}
			entriesA.push_back(entry);
		}


		size = groupB.size();
		vector<GRtEntry> entriesB;
		for(u32 i=0;i<size;i++)
		{
			entriesB.push_back(Children[groupB[i]]);
		}

		Bound = boundA;
		Children = entriesA;
		
		GRtIndex* newIndex = new GRtIndex(Owner,Parent,Owner->nextId(),boundB,entriesB);
	
		for(u32 i=0;i<size;i++)
		{
			if(entriesB[i].Node!=NULL)
			{
				entriesB[i].Node->Release();
			}
		}


		if(Parent!=NULL)
		{
			Parent->adjustTree(this);
			Parent->addChildNode(newIndex);
			Owner->getCache()->addNode(newIndex);

			newIndex->Release();

		}
		else
		{
			vector<GRtEntry> entries;
			
			GRtEntry entry;
			entry.Id = Id;
			entry.Bound = Bound;
			entry.Node = this;
			entries.push_back(entry);
			
			entry.Id = newIndex->getId();
			entry.Bound = newIndex->getBound();
			entry.Node = newIndex;
			entries.push_back(entry);

			rect<f32> newBound = combineBounds<f32>(boundA,boundB);
			GRtIndex* newRoot = new GRtIndex(Owner,NULL,Owner->nextId(),newBound,entries);
			
			Owner->switchRoot(newRoot);
			

			Owner->getCache()->addNode(newIndex);
			Owner->getCache()->addNode(newRoot);

			newRoot->Release();
			newIndex->Release();
		}

	}
	else
	{	
		Bound = combineBounds<f32>(Bound,node->getBound());
		if(Parent!=NULL)
			Parent->adjustTree(this);
	}
	setDirty();

}

//!调整节点
void GRtIndex::adjustTree(GRtNode* node)
{
	Children[node->getIndex()].Bound = node->getBound();
	rect<f32> newBound = rtree::combineBounds<GRtEntry>(Children);

	if(newBound  != Bound)
	{
		Bound = newBound;		
		if(Parent!=NULL)
		{
			Parent->adjustTree(this);
		}
	}
	setDirty();
}

//!confenseTree
void GRtIndex::confenseTree(vector<GRtIndex*>& toReinserts,GRtNode* deletedNode)
{
	Children.erase(Children.begin()+deletedNode->getIndex());

	for(u32 i=0;i<Children.size();i++)
	{
		Children[i].Node->setIndex(i);
	}

	rect<f32> newBound = rtree::combineBounds<GRtEntry>(Children);

	if(newBound  != Bound)
	{
		Bound = newBound;		
		if(Parent!=NULL)
		{
			Parent->adjustTree(this);
		}
	}
	
	if(Children.size()<Owner->getMinChildCount())
	{
		if(Parent == NULL)
		{	
			if(Children.size()==1 && !deletedNode->isLeaf())
			{	
				Owner->switchRoot(static_cast<GRtIndex*>(getChildNode(0)));
				Owner->getCache()->deleteNode(this);
			}
			else if(Children.size()==0)
			{
				Owner->switchRoot(NULL);
				Owner->getCache()->deleteNode(this);			
			}
		}
		else
		{
			toReinserts.push_back(this);
			Parent->confenseTree(toReinserts,this);
		}
	}
 }

}}