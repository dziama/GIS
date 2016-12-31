#pragma once

#include "Edge.h"
#include "Vertex.h"
#include "HeapNode.h"

typedef shared_ptr<IHeapNode> NodePtr;
typedef pair<VertexId, NodePtr> HeapPair;
typedef map<VertexId, NodePtr> HeapNodes;

class FibonacciHeap
{
public:
	FibonacciHeap();
	~FibonacciHeap();

	void insert(VertexPtr ptr, EdgeWeight weight);
	VertexPtr extractMin();
	void printRootList(ostream& stream);

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
	void heapLink();
	void consolidate();
	bool isEmpty();

	bool pointerEmpty(const HeapNodePtr& ptr);

	void insertIntoRootList(NodePtr node);
	VertexPtr removeMinFromRootList();
};

