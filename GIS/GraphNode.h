#pragma once
#include "Interfaces.h"

class GraphNode : IGraphNode
{
	virtual GraphNodePtr getParent() override;
	virtual void setParent(GraphNodePtr ptr) override;

	virtual GraphNodePtr getChild() override;
	virtual void setChild(GraphNodePtr ptr) override;

	virtual NodeDegree getDegree() override;
	virtual void setDegree(NodeDegree degree) override;

	virtual void setNext(GraphNodePtr ptr) override;
	virtual GraphNodePtr getNext() override;

	virtual void setPrev(GraphNodePtr ptr) override;
	virtual GraphNodePtr getPrev() override;

	virtual VertexPtr getVertex() override;
	virtual bool isMarked() override;
	virtual void mark() override;

	virtual bool hasChild() override;
	virtual bool hasParent() override;
};