#pragma once

#include "Edge.h"
#include "Vertex.h"
#include "HeapNode.h"

class FibonacciHeap
{
	friend class FibbonacciHeapTestClass;

public:
	FibonacciHeap();
	~FibonacciHeap();

	void insert(VertexPtr ptr, EdgeWeight weight);
	NodePtr extractMin();
	void printNodeList(ostream& stream, HeapNodePtr& ptr, bool verbose = false);
	HeapNodePtr peekMinElement();

	size_t getSize();

private:
	HeapNodePtr m_MinElement;

	HeapNodes m_HeapNodes;

	NodeId m_NextFreeNodeNumber;

	//void decreaseKey();
	//void cut(HeapNodePtr child, HeapNodePtr parent);
	//void cascadeCut(HeapNodePtr node);
	//void deleteNodes(HeapNodePtr node);
	//void merge(HeapNodePtr a, HeapNodePtr b);
	//void findInHeap();
	void heapLink(NodePtr& toChild, NodePtr& toParent);
	void consolidate();
	bool isEmpty();

	bool pointerEmpty(const HeapNodePtr& ptr);

	void insertIntoRootList(NodePtr& node);
	void removeFromRootList(NodePtr& ptr);
	NodePtr removeMinFromRootList();

};

