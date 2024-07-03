#include "GRtNode.h"

namespace gmap { namespace rtree{

	GRtNode::GRtNode(GRTree* owner,u32 id)
	{
		Owner = owner;	
		Parent = NULL;
		Id = id;
		Index = 0xFFFFFFFF;
		Dirty = true;
	}

	GRtNode::~GRtNode()
	{

	}

	//!��ȡӵ����
	GRTree* GRtNode::getOwner()
	{
		return Owner;
	}

	//!��ȡId
	u32 GRtNode::getId()
	{
		return Id;
	}

	//!��ȡ����
	u32 GRtNode::getIndex()
	{
		return Index;
	}

	//!��������
	void GRtNode::setIndex(u32 index)
	{
		Index = index;
	}

	//!��ȡ���׽ڵ�
	GRtIndex* GRtNode::getParent()
	{
		return Parent;
	}

	//!���ø��ڵ�
	void GRtNode::setParent(GRtIndex* parent)
	{
		Parent = parent;
	}

	//!���Dirty
	void GRtNode::clearDirty()
	{
		Dirty = false;
	}

	//!����Dirty
	void GRtNode::setDirty()
	{
		Dirty = true;
	}


	bool GRtNode::getDirty()
	{
		return Dirty;
	}

}}
