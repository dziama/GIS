#include "FibbonacciHeapTestClass.h"

FibbonacciHeapTestClass::FibbonacciHeapTestClass()
{
}


FibbonacciHeapTestClass::~FibbonacciHeapTestClass()
{
}

void FibbonacciHeapTestClass::testExtractMin()
{
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15));
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[2], 1));

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0])));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1])));

	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be 1 15" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	auto min = heap.extractMin();
	if (min->getPriority() != 1)
	{
		throw exception{ "Heap returned wrong min element!" };
	}

	verifyNodePointer(min);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	heap.printNodeList(std::cout, heap.peekMinElement(), true);


	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[2], vertex_ids[3], 16));
	edge_ids.push_back(graph.addEdge(vertex_ids[3], vertex_ids[5], 10));
	edge_ids.push_back(graph.addEdge(vertex_ids[5], vertex_ids[4], 22));
	edge_ids.push_back(graph.addEdge(vertex_ids[2], vertex_ids[4], 37));

	heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2]));
	heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3]));
	heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4]));
	heap.insert(graph.getVertex(vertex_ids[5]), graph.getEdge(edge_ids[5]));

	std::cout << "Should be 10 15 16 22 37" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	min = heap.extractMin();

	verifyNodePointer(min);
	if (min->getDegree() != 0)
	{
		throw exception{ "Heap returned wrong min element with wrong degree!" };
	}

	if (min->getPriority() != 10)
	{
		throw exception{ "Heap returned wrong min element!" };
	}

	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be 15" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	auto child = heap.peekMinElement().lock()->getChild();
	verifyNodeDoubleLinkedList(child.lock());

	auto second_order_child = child.lock()->getChild();

	verifyNodePointer(second_order_child.lock());

	if (heap.peekMinElement().lock()->getPriority() != 15)
	{
		throw exception{ "Heap points to wrong min element!" };
	}
}

void FibbonacciHeapTestClass::testHeapLink()
{
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeId> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15));
	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[2], 1));
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[1], 12));

	for (unsigned i = 0; i < vertex_ids.size(); ++i)
	{
		heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[i]), graph.getEdge(edge_ids[i])));
	}

	std::cout << "Should be 1 15 12" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	auto min = heap.peekMinElement().lock();
	if (min->getPriority() != 1)
	{
		throw exception{ "Heap returned wrong priority min element!" };
	}

	verifyNodePointer(min);

	auto parent = heap.m_HeapNodes[min->getNodeNumber()];

	verifyNodePointer(parent);

	auto child = heap.m_HeapNodes[heap_node_ids[2]];

	verifyNodePointer(child);

	heap.heapLink(child, parent);

	verifyNodeDoubleLinkedList(parent);
	verifyNodePointer(parent);
	verifyNodeDoubleLinkedList(child);

	std::cout << "Should be 1 15" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	std::cout << "Should be 12" << endl;
	heap.printNodeList(std::cout, parent->getChild());

	if (parent->hasChild() != true)
	{
		throw exception{ "Parent says he has no children!" };
	}

	if (child->hasParent() != true)
	{
		throw exception{ "Child says he has no parent!" };
	}

	if (parent->getDegree() != 1)
	{
		throw exception{ "Wrong parent degree!" };
	}

	if (child->getDegree() != 0)
	{
		throw exception{ "Wrong child degree!" };
	}

	auto second_child = heap.m_HeapNodes[heap_node_ids[0]];

	verifyNodePointer(second_child);

	heap.heapLink(second_child, parent);

	verifyNodeDoubleLinkedList(parent);
	verifyNodePointer(parent);
	verifyNodeDoubleLinkedList(parent->getChild().lock());

	std::cout << "Should be 1" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	std::cout << "Should be 15, 12" << endl;
	heap.printNodeList(std::cout, parent->getChild());

	if (parent->hasChild() != true)
	{
		throw exception{ "Parent says he has no children!" };
	}

	if (child->hasParent() != true)
	{
		throw exception{ "Child says he has no parent!" };
	}

	if (parent->getDegree() != 2)
	{
		throw exception{ "Wrong parent degree!" };
	}
}

void FibbonacciHeapTestClass::testMoveNodeChildren()
{
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15));
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[2], 1));
	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[2], 12));

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0])));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1])));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2])));

	auto parent = heap.m_HeapNodes[heap_node_ids[1]];

	verifyNodePointer(parent);

	auto child = heap.m_HeapNodes[heap_node_ids[2]];

	verifyNodePointer(child);

	heap.heapLink(child, parent);

	auto second_child = heap.m_HeapNodes[heap_node_ids[0]];

	verifyNodePointer(second_child);

	heap.heapLink(second_child, parent);

	verifyNodeDoubleLinkedList(parent);
	verifyNodePointer(parent);
	verifyNodeDoubleLinkedList(parent->getChild().lock());

	std::cout << "Should be 1" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	std::cout << "Should be 15, 12" << endl;
	heap.printNodeList(std::cout, parent->getChild());

	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[3], vertex_ids[4], 6));
	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[3], 5));

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3])));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4])));

	std::cout << "Should be 1 6 5" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	heap.moveNodeChildrenToRootList(parent);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	auto min = heap.peekMinElement().lock();
	if (min->getPriority() != 5)
	{
		throw exception{ "Heap returned wrong priority min element!" };
	}

	std::cout << "Should be 5 15 12 6" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	heap.moveNodeChildrenToRootList(heap.m_HeapNodes[heap_node_ids[4]]);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be  6 15 12" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());
}