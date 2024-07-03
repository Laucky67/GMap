#ifndef __SE_RT_INDEX_H__
#define __SE_RT_INDEX_H__

#include "GTypes.h"
#include "GCoreBase.h"
#include "GPosition2d.h"
#include "GRect.h"

#include "GRtNode.h"
#include "GRtDefs.h"

namespace gmap
{
namespace rtree
{	
	class GRtNode;
	class GRtLeaf;
	class GRTree;

	struct GRtEntry
	{
		u32 Id;
		u32 IndexKey;
		rect<f32> Bound;
		GRtNode* Node;

		rect<f32>& getBound(){return Bound;}

		bool operator <(const GRtEntry& other)
		{			
			if (IndexKey<other.IndexKey)
			{
				return true;
			}
			else if(IndexKey==other.IndexKey)
			{
				return Bound.getCenter().Y<other.Bound.getCenter().Y;
			}
			else
			{
				return false;
			}
		}
	};

	class GRtIndex:public GRtNode
	{
	public:
		GRtIndex(GRTree* owner,GRtIndex* parent,u32 id,const rect<f32>& bound,const vector<GRtEntry>& children);

		~GRtIndex();

	public:
		 const rect<f32>& getBound();

		//!�Ƿ�Ҷ�ӽڵ�
		 bool isLeaf();

		//!�Ƿ���ڵ�
		 bool isRoot();

		//!������
		 void visit(IRtVisitor* visitor,vector<GRtNode*>& path);

		//!�ж��ܷ�ж��
		 bool canUnload();

		 void sort();

	public:
		//!��ȡ���ӵĸ���
		u32 getChildrenCount();

		//!��ȡ���ӵ�Id
		u32 getChildId(u32 index);

		//!��ȡ���ӵķ�Χ
		const rect<f32>& getChildBound(u32 index);

		//!��ȡ���ӽڵ�
		GRtNode* getChildNode(u32 index);

		//!ѡ��Ҷ�ӽڵ�
		GRtLeaf*	chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path);

		//!ѡ�������ڵ�
		GRtIndex*  chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter);

		//!����Ҷ�ӽڵ�
		GRtLeaf*  searchLeaf(u32 id,const rect<f32>& bound);

		//!����½ڵ�
		void addChildNode(GRtNode* node);

		//!�����ڵ�
		void adjustTree(GRtNode* node);

		//!������
		void confenseTree(vector<GRtIndex*>& toReinserts,GRtNode* deletedNode);
	
		//!��ȡ���ĸ߶�
		u32 getHeight();

		//!��ȡ�������
		u32 getDepth();

		//!����
		void visitDepthFirst(IRtVisitor* visitor,vector<GRtNode*>& path);

	private:
		void unloadChild(u32 index);

	private:
		vector<GRtEntry> Children;

		rect<f32>		 Bound;

	};
}}

#endif 