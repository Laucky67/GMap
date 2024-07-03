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
		//!���캯��
		GRtLeaf(GRTree* owner, GRtIndex* parent,u32 id,const rect<f32>& bound,const vector<GRtItem>& items);
		
		//!��������
		~GRtLeaf();
	public:
		//!��ȡ��Χ��
		 const rect<f32>& getBound();

		//!�Ƿ�Ҷ�ӽڵ�
		 bool isLeaf();

		//!�Ƿ���ڵ�
		 bool isRoot();

		//!�ж��ܷ�ж��
		 bool canUnload();	

		 void sort();
	public:
		//!������Ŀ
		 virtual void insertItem(IRtItem* pItem);

		//!ɾ����Ŀ
		 virtual void deleteItem(IRtItem* pItem);

		 //!��ȡ���������е���Ŀ
		 vector<GRtItem>& getItems();
	private:
		//!��������Ŀ
		vector<GRtItem> Items;
		//!��Χ����
		rect<f32>	Bound;
	};

	

}}
#endif