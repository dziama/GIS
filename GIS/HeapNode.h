#pragma once
#include "Interfaces.h"

class HeapNode : public IHeapNode
{
public:
	virtual HeapNodePtr getParent() override;
	virtual void setParent(HeapNodePtr ptr) override;

	virtual HeapNodePtr getChild() override;
	virtual void setChild(HeapNodePtr ptr) override;

	virtual NodeDegree getDegree() override;
	//virtual void setDegree(NodeDegree degree) override;
	virtual void increaseDegree(NodeDegree increase) override;

	virtual void setNext(HeapNodePtr ptr) override;
	virtual HeapNodePtr getNext() override;

	virtual void setPrev(HeapNodePtr ptr) override;
	virtual HeapNodePtr getPrev() override;

	virtual VertexPtr getVertex() override;
	virtual bool isMarked() override;
	virtual void mark() override;
	virtual void unmark() override;

	virtual bool hasChild() override;
	virtual bool hasParent() override;

	virtual NodeId getNodeNumber() override;

	virtual EdgeWeight getPriority();

	//HeapNode(VertexPtr ptr, EdgeWeight priority, HeapNodePtr next, HeapNodePtr prev);
	HeapNode(VertexPtr ptr, EdgeWeight priority, NodeId num);

private:

	HeapNodePtr m_Parent;
	HeapNodePtr m_Child;
	HeapNodePtr m_Next;
	HeapNodePtr m_Prev;

	VertexPtr m_Vertex;

	NodeId m_NodeNumber;

	bool m_Marked;

	NodeDegree m_Degree;

	EdgeWeight m_Priority;
};