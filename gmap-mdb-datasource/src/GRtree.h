#ifndef __RTREE_H__
#define __RTREE_H__

#include "GTypes.h"
#include "GCoreBase.h"
#include "GPosition2d.h"
#include "GRect.h"
#include "GDimension2d.h"
#include "GRTDefs.h"

namespace gmap
{
namespace rtree
{
	class GRtIndex;
	class GRtNode;
	class GRtLeaf;
	class IRTreeSite;
	class IRtVisitor;	

	class GRTree
	{
	public:
		//!构造函数
		GRTree();

		//!析构函数
		~GRTree();
		
		//!初始化
		void initialize(IRTreeSite* cache,u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount);
	
		//!初始化
		void initialize(IRTreeSite* cache);

		//!初始化
		void uninitialize();

		//!保存所有数据
		void save();

		//!获取Cache
		IRTreeSite* getCache();

		//!获取下一个Id
		u32 nextId();
		
		//!获取子节点数
		u32 getMaxChildCount();

		//!获取最小子节点数量
		u32 getMinChildCount();

		//!获取最大的项目数
		u32 getMaxItemCount();

		//!获取最小的项目数
		u32 getMinItemCount();

		//!获取根节点
		GRtIndex* getRoot();

		//!遍历
		void visit(IRtVisitor* visitor);

		//!遍历
		void visitDepthFirst(IRtVisitor* visitor);

		//!插入数据项
		void insertItem(IRtItem* item);

		//!删除数据项
		void deleteItem(IRtItem* item);

		//!选择叶子节点
		GRtLeaf*	chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path);

		//!选择索引节点
		GRtIndex*  chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter);

		//!查找叶子节点
		GRtLeaf*  searchLeaf(u32 id,const rect<f32>& bound);

		//!获取树的深度
		u32 getDepth();

		//!进行希尔伯特排序
		void BuildIndexKey();

	private:
		void switchRoot(GRtIndex* root);
		//!得到排序码
		u32 GetHbCode(f64 x,f64 y);

		friend  GRtIndex;
		friend  GRtLeaf;
		friend  GRtNode;
	public:
		u32 RootId;

		GRtIndex* Root;

		IRTreeSite* Cache;

		u32 MinItemCount;
		
		u32 MaxItemCount;
		
		u32 MinChildCount;
		
		u32 MaxChildCount;

		rect<f64> HbBound;

		f64		  HbCellSize;
		u32		  HbLevelCount;

		//CHilbertMaker HiblertMaker;
	};

}}
#endif