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

	//!获取拥有者
	GRTree* GRtNode::getOwner()
	{
		return Owner;
	}

	//!获取Id
	u32 GRtNode::getId()
	{
		return Id;
	}

	//!获取索引
	u32 GRtNode::getIndex()
	{
		return Index;
	}

	//!设置索引
	void GRtNode::setIndex(u32 index)
	{
		Index = index;
	}

	//!获取父亲节点
	GRtIndex* GRtNode::getParent()
	{
		return Parent;
	}

	//!设置父节点
	void GRtNode::setParent(GRtIndex* parent)
	{
		Parent = parent;
	}

	//!清除Dirty
	void GRtNode::clearDirty()
	{
		Dirty = false;
	}

	//!设置Dirty
	void GRtNode::setDirty()
	{
		Dirty = true;
	}


	bool GRtNode::getDirty()
	{
		return Dirty;
	}

}}
