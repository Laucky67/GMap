#ifndef __G_RT_LEAF_H__
#define __G_RT_LEAF_H__


#include "GTypes.h"
#include "GCoreBase.h"
#include "GPosition2d.h"
#include "GRect.h"

#include "GRtNode.h"

#include "GRtNode.h"
#include "GRtDefs.h"

namespace gmap
{
namespace rtree
{
	class GRtLeaf:public GRtNode
	{
	public:
		//!构造函数
		GRtLeaf(GRTree* owner, GRtIndex* parent,u32 id,const rect<f32>& bound,const vector<GRtItem>& items);
		
		//!析构函数
		~GRtLeaf();
	public:
		//!获取包围盒
		 const rect<f32>& getBound();

		//!是否叶子节点
		 bool isLeaf();

		//!是否根节点
		 bool isRoot();

		//!判断能否卸载
		 bool canUnload();	

		 void sort();
	public:
		//!插入项目
		 virtual void insertItem(IRtItem* pItem);

		//!删除项目
		 virtual void deleteItem(IRtItem* pItem);

		 //!获取容器中所有的项目
		 vector<GRtItem>& getItems();
	private:
		//!包含的项目
		vector<GRtItem> Items;
		//!包围盒子
		rect<f32>	Bound;
	};

	

}}
#endif