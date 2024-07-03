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

		//!是否叶子节点
		 bool isLeaf();

		//!是否根节点
		 bool isRoot();

		//!遍历树
		 void visit(IRtVisitor* visitor,vector<GRtNode*>& path);

		//!判断能否卸载
		 bool canUnload();

		 void sort();

	public:
		//!获取孩子的个数
		u32 getChildrenCount();

		//!获取孩子的Id
		u32 getChildId(u32 index);

		//!获取孩子的范围
		const rect<f32>& getChildBound(u32 index);

		//!获取孩子节点
		GRtNode* getChildNode(u32 index);

		//!选择叶子节点
		GRtLeaf*	chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path);

		//!选择索引节点
		GRtIndex*  chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter);

		//!查找叶子节点
		GRtLeaf*  searchLeaf(u32 id,const rect<f32>& bound);

		//!添加新节点
		void addChildNode(GRtNode* node);

		//!调整节点
		void adjustTree(GRtNode* node);

		//!精简树
		void confenseTree(vector<GRtIndex*>& toReinserts,GRtNode* deletedNode);
	
		//!获取树的高度
		u32 getHeight();

		//!获取树的深度
		u32 getDepth();

		//!遍历
		void visitDepthFirst(IRtVisitor* visitor,vector<GRtNode*>& path);

	private:
		void unloadChild(u32 index);

	private:
		vector<GRtEntry> Children;

		rect<f32>		 Bound;

	};
}}

#endif 