#pragma once

#include "Edge.h"
#include "Vertex.h"
#include "HeapNode.h"
#include "Utility.h"

class FibonacciHeap;

typedef shared_ptr<FibonacciHeap> HeapPtr;

class FibonacciHeap
{
	friend class FibbonacciHeapTestClass;

public:
	FibonacciHeap();
	~FibonacciHeap();

	HeapNodeId insert(VertexPtr ptr, EdgePtr edge);
	NodePtr extractMin();
	void printNodeList(ostream& stream, HeapNodePtr& ptr, bool verbose = false);
	void printNode(ostream& stream, HeapNodePtr& ptr);

	HeapNodePtr peekMinElement();
	bool isEmpty();
	size_t getSize();

	HeapNodes m_HeapNodes;

private:
	HeapNodePtr m_MinElement;

	

	HeapNodeId m_NextFreeNodeNumber;

	//void decreaseKey();
	//void cut(HeapNodePtr child, HeapNodePtr parent);
	//void cascadeCut(HeapNodePtr node);
	//void deleteNodes(HeapNodePtr node);
	//void merge(HeapNodePtr a, HeapNodePtr b);
	//void findInHeap();
	void heapLink(NodePtr& toChild, NodePtr& toParent);
	void consolidate(NodePtr begin);
	

	bool pointerEmpty(const HeapNodePtr& ptr);

	void insertIntoRootList(NodePtr& node);
	void removeFromRootList(NodePtr& ptr);
	NodePtr removeMinFromRootList();
	NodePtr moveNodeChildrenToRootList(NodePtr& node);

};

