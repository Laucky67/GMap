#ifndef __G_RT_VISITOR_H__
#define __G_RT_VISITOR_H__

#include "GRtNode.h"
#include "GRtLeaf.h"
#include "GRtIndex.h"

namespace gmap{
	namespace rtree{

		//!对R树内的节点进行排序
		class GRtHbKeySettor:public IRtVisitor
		{
		public:
			GRtHbKeySettor()
			{

			}
			virtual bool executeFilter(const rect<f32>& bound)
			{
				return true;
			}

			virtual s32 getTargetLevel()
			{
				return -2;
			}

			virtual void visitNode(GRtNode* node,vector<GRtNode*>& path)
			{
				if(node->isLeaf())
				{			
					GRtLeaf *pLeaf= static_cast<GRtLeaf*>(node);
					pLeaf->sort();
				}
				else
				{
					GRtIndex *pIndex= static_cast<GRtIndex*>(node);
					pIndex->sort();
				}
			}

			virtual bool execute(const rect<f32>& bound)
			{
				return true;
			}
		};


		class GRtItemIndexKeySettor:public IRtVisitor
		{
		public:

			u32 IndexKey;
			GRtItemIndexKeySettor()
			{
				IndexKey =0;
			}
			virtual bool executeFilter(const rect<f32>& bound)
			{
				return true;
			}

			virtual s32 getTargetLevel()
			{
				return -1;
			}

			virtual void visitNode(GRtNode* node,vector<GRtNode*>& path)
			{
				GRtLeaf *pLeaf= static_cast<GRtLeaf*>(node);

				//!获取容器中所有的项目
				vector<GRtItem>& items= pLeaf->getItems();

				u32 sz = items.size();
				for(u32 i=0;i<sz;i++)
				{
					GRtItem& item = items[i];
					item.IndexKey = ++IndexKey;
				}
			}
		};

	}}
#endif