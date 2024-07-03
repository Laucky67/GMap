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
		//!���캯��
		GRTree();

		//!��������
		~GRTree();
		
		//!��ʼ��
		void initialize(IRTreeSite* cache,u32 minChildCount,u32 maxChildCount,u32 minItemCount,u32 maxItemCount);
	
		//!��ʼ��
		void initialize(IRTreeSite* cache);

		//!��ʼ��
		void uninitialize();

		//!������������
		void save();

		//!��ȡCache
		IRTreeSite* getCache();

		//!��ȡ��һ��Id
		u32 nextId();
		
		//!��ȡ�ӽڵ���
		u32 getMaxChildCount();

		//!��ȡ��С�ӽڵ�����
		u32 getMinChildCount();

		//!��ȡ������Ŀ��
		u32 getMaxItemCount();

		//!��ȡ��С����Ŀ��
		u32 getMinItemCount();

		//!��ȡ���ڵ�
		GRtIndex* getRoot();

		//!����
		void visit(IRtVisitor* visitor);

		//!����
		void visitDepthFirst(IRtVisitor* visitor);

		//!����������
		void insertItem(IRtItem* item);

		//!ɾ��������
		void deleteItem(IRtItem* item);

		//!ѡ��Ҷ�ӽڵ�
		GRtLeaf*	chooseLeaf(const rect<f32>& bound,vector<GRtNode*>& path);

		//!ѡ�������ڵ�
		GRtIndex*  chooseIndex(const rect<f32>& bound,vector<GRtNode*>& path,u32 levelCounter);

		//!����Ҷ�ӽڵ�
		GRtLeaf*  searchLeaf(u32 id,const rect<f32>& bound);

		//!��ȡ�������
		u32 getDepth();

		//!����ϣ����������
		void BuildIndexKey();

	private:
		void switchRoot(GRtIndex* root);
		//!�õ�������
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