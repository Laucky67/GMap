#ifndef __G_RT_NODE_H__
#define __G_RT_NODE_H__

#include "GTypes.h"
#include "GCoreBase.h"
#include "GPosition2d.h"
#include "GRect.h"
#include "GUnknown.h"

namespace gmap
{
namespace rtree
{
	class IRtVisitor;
	class IRtFilter;
	class GRtIndex;
	class GRTree;


	class GRtNode:public GUnknown
	{
	public:
		GRtNode(GRTree* owner,u32 id);

		~GRtNode();

		//!获取拥有者
		GRTree* getOwner();

		//!获取Id
		u32 getId();

		//!获取索引
		u32 getIndex();

		//!设置索引
		void setIndex(u32 index);

		//!获取父亲节点
		GRtIndex* getParent();

		//!设置父节点
		void setParent(GRtIndex* parent);

		//!获取Bound;
		virtual const rect<f32>& getBound()=0;

		//!是否叶子节点
		virtual  bool isLeaf()=0;

		//!是否根节点
		virtual  bool isRoot()=0;

		//!判断能否卸载
		virtual  bool canUnload()=0;

		////////////////////////////////
		//!获取是否是脏数据
		bool getDirty();

		//!清除脏数据
		void clearDirty();
		
		//!设置数据为脏
		void setDirty();

	protected:
		u32 Id;
		u32 Index;
		GRTree*   Owner;
		GRtIndex* Parent;
		bool Dirty;
	};
}}
#endif