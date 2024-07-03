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

		//!��ȡӵ����
		GRTree* getOwner();

		//!��ȡId
		u32 getId();

		//!��ȡ����
		u32 getIndex();

		//!��������
		void setIndex(u32 index);

		//!��ȡ���׽ڵ�
		GRtIndex* getParent();

		//!���ø��ڵ�
		void setParent(GRtIndex* parent);

		//!��ȡBound;
		virtual const rect<f32>& getBound()=0;

		//!�Ƿ�Ҷ�ӽڵ�
		virtual  bool isLeaf()=0;

		//!�Ƿ���ڵ�
		virtual  bool isRoot()=0;

		//!�ж��ܷ�ж��
		virtual  bool canUnload()=0;

		////////////////////////////////
		//!��ȡ�Ƿ���������
		bool getDirty();

		//!���������
		void clearDirty();
		
		//!��������Ϊ��
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