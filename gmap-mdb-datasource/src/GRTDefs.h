#ifndef __G_RT_DEFS_H__
#define __G_RT_DEFS_H__

#include "GCoreBase.h"
#include "GRect.h"

namespace gmap{
	namespace rtree{

		class GRTreeNode;
		class GRtLeaf;
		class GRtNode;
		class GRTree;

		class IRtItem
		{
		public:
			virtual u32			getId()=0;
			virtual rect<f32>	getBound()=0;
			virtual void		setOwner(GRtLeaf* leaf,u32 ownerIndex)=0;
			virtual GRtLeaf*	getOwner()=0;
			virtual u32			getOwnerIndex()=0;
		};

		struct GRtItem
		{
			//!¼ü
			u32			IndexKey;
			IRtItem*	Item;

			rect<f32>	getBound()
			{
				return Item->getBound();
			}

			u32			getId(){return Item->getId();}

			bool operator <(const GRtItem& other)const
			{			
				if (IndexKey<other.IndexKey)
				{
					return true;
				}
				else if(IndexKey==other.IndexKey)
				{
					return Item->getBound().getCenter().Y<other.Item->getBound().getCenter().Y;
				}
				else
				{
					return false;
				}
			}
		};


		//!RTreeSite
		class IRTreeSite
		{
		public:
			virtual GRtNode* loadNode(GRTree* owner,u32 id)=0;

			virtual void save()=0;

			virtual void deleteNode(GRtNode* node)=0;

			virtual void addNode(GRtNode* node)=0;

			virtual bool isInitialized() =0;

			virtual u32 nextId()=0;

			virtual void getInitialParams(u32& rootId,u32& minChildCount,u32& maxChildCount,u32& minItemCount,u32& maxItemCount)=0;

			virtual void initialize(u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount)=0;
		};

		class IRtVisitor
		{
		public:

			virtual bool executeFilter(const rect<f32>& bound)=0;

			virtual s32 getTargetLevel()=0;

			virtual void visitNode(GRtNode* node,vector<GRtNode*>& path)=0;
		};

	}}

#endif