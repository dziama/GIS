#include "FibbonacciHeapTestClass.h"

FibbonacciHeapTestClass::FibbonacciHeapTestClass()
{
}


FibbonacciHeapTestClass::~FibbonacciHeapTestClass()
{
}

void FibbonacciHeapTestClass::testExtractMin()
{
	std::cout << "__________________EXTRACT_MIN_TEST_________________" << endl;
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

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0]).lock()->getWeight()));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1]).lock()->getWeight()));

	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be 1 15" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	auto min = heap.extractMin();
	if (min->getPriority() != 1)
	{
		throw runtime_error{ "Heap returned wrong min element!" };
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

	heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2]).lock()->getWeight());
	heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3]).lock()->getWeight());
	heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4]).lock()->getWeight());
	heap.insert(graph.getVertex(vertex_ids[5]), graph.getEdge(edge_ids[5]).lock()->getWeight());

	std::cout << "Should be 10 15 16 22 37" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	min = heap.extractMin();

	verifyNodePointer(min);
	if (min->getDegree() != 0)
	{
		throw runtime_error{ "Heap returned wrong min element with wrong degree!" };
	}

	if (min->getPriority() != 10)
	{
		throw runtime_error{ "Heap returned wrong min element!" };
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
		throw runtime_error{ "Heap points to wrong min element!" };
	}
}

void FibbonacciHeapTestClass::testHeapLink()
{
	std::cout << "__________________HEAP_LINK_TEST_________________" << endl;
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
		heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[i]), graph.getEdge(edge_ids[i]).lock()->getWeight()));
	}

	std::cout << "Should be 1 15 12" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	auto min = heap.peekMinElement().lock();
	if (min->getPriority() != 1)
	{
		throw runtime_error{ "Heap returned wrong priority min element!" };
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
		throw runtime_error{ "Parent says he has no children!" };
	}

	if (child->hasParent() != true)
	{
		throw runtime_error{ "Child says he has no parent!" };
	}

	if (parent->getDegree() != 1)
	{
		throw runtime_error{ "Wrong parent degree!" };
	}

	if (child->getDegree() != 0)
	{
		throw runtime_error{ "Wrong child degree!" };
	}

	auto second_child = heap.m_HeapNodes[heap_node_ids[0]];

	verifyNodePointer(second_child);

	heap.heapLink(second_child, parent);

	verifyNodeDoubleLinkedList(parent);
	verifyNodePointer(parent);
	verifyNodeDoubleLinkedList(parent->getChild().lock());

	std::cout << "Should be 1" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	std::cout << "Should be 12, 15" << endl;
	heap.printNodeList(std::cout, parent->getChild());

	if (parent->hasChild() != true)
	{
		throw runtime_error{ "Parent says he has no children!" };
	}

	if (child->hasParent() != true)
	{
		throw runtime_error{ "Child says he has no parent!" };
	}

	if (parent->getDegree() != 2)
	{
		throw runtime_error{ "Wrong parent degree!" };
	}
}

void FibbonacciHeapTestClass::testMoveNodeChildren()
{
	std::cout << "__________________MOVE_NODE_CHILDREN_TEST_________________" << endl;
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

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0]).lock()->getWeight()));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1]).lock()->getWeight()));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2]).lock()->getWeight()));

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

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3]).lock()->getWeight()));
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4]).lock()->getWeight()));

	std::cout << "Should be 1 6 5" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	heap.moveNodeChildrenToRootList(parent);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	auto min = heap.peekMinElement().lock();
	if (min->getPriority() != 5)
	{
		throw runtime_error{ "Heap returned wrong priority min element!" };
	}

	std::cout << "Should be 5 15 12 6" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());

	heap.moveNodeChildrenToRootList(heap.m_HeapNodes[heap_node_ids[4]]);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be  6 15 12" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());
}

void FibbonacciHeapTestClass::testCut()
{
	std::cout << "__________________CUT_TEST_________________" << endl;
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex()); //0
	vertex_ids.push_back(graph.addVertex()); //1
	vertex_ids.push_back(graph.addVertex()); //2
	vertex_ids.push_back(graph.addVertex()); //3
	vertex_ids.push_back(graph.addVertex()); //4
	vertex_ids.push_back(graph.addVertex()); //5
	vertex_ids.push_back(graph.addVertex()); //6

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15)); //0
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[2], 1)); //1
	edge_ids.push_back(graph.addEdge(vertex_ids[2], vertex_ids[3], 10)); //2
	edge_ids.push_back(graph.addEdge(vertex_ids[4], vertex_ids[0], 13)); //3
	edge_ids.push_back(graph.addEdge(vertex_ids[3], vertex_ids[1], 17)); //4
	edge_ids.push_back(graph.addEdge(vertex_ids[5], vertex_ids[3], 9)); //5
	edge_ids.push_back(graph.addEdge(vertex_ids[6], vertex_ids[2], 8)); //6

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0]).lock()->getWeight())); //0 15
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1]).lock()->getWeight())); //1 1
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2]).lock()->getWeight())); //2 10
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3]).lock()->getWeight())); //3 13
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4]).lock()->getWeight())); //4 17
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[5]), graph.getEdge(edge_ids[5]).lock()->getWeight())); //5 9
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[6]), graph.getEdge(edge_ids[6]).lock()->getWeight())); //6 8

	heap.extractMin();

	std::cout << "Should be  8 10" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	//cut node with weight 13 from node 10 parent
	auto parent = heap.find(heap_node_ids[2]);
	auto child = heap.find(heap_node_ids[3]);

	if (parent->getPriority() != 10)
	{
		throw runtime_error{ "Heap returned wrong priority element!" };
	}

	if (child->getPriority() != 13)
	{
		throw runtime_error{ "Heap returned wrong priority element!" };
	}

	heap.cut(child, parent);

	std::cout << "Should be  8 10 13" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());
}

void FibbonacciHeapTestClass::testDecreaseKey()
{
	std::cout << "__________________DECREASE_KEY_TEST_________________" << endl;
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex()); //0
	vertex_ids.push_back(graph.addVertex()); //1
	vertex_ids.push_back(graph.addVertex()); //2
	vertex_ids.push_back(graph.addVertex()); //3
	vertex_ids.push_back(graph.addVertex()); //4
	vertex_ids.push_back(graph.addVertex()); //5
	vertex_ids.push_back(graph.addVertex()); //6
	vertex_ids.push_back(graph.addVertex()); //7
	vertex_ids.push_back(graph.addVertex()); //8

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15)); //0
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[2], 1)); //1
	edge_ids.push_back(graph.addEdge(vertex_ids[2], vertex_ids[3], 10)); //2
	edge_ids.push_back(graph.addEdge(vertex_ids[4], vertex_ids[0], 13)); //3
	edge_ids.push_back(graph.addEdge(vertex_ids[3], vertex_ids[1], 17)); //4
	edge_ids.push_back(graph.addEdge(vertex_ids[5], vertex_ids[3], 9)); //5
	edge_ids.push_back(graph.addEdge(vertex_ids[6], vertex_ids[2], 8)); //6
	edge_ids.push_back(graph.addEdge(vertex_ids[5], vertex_ids[3], 11)); //7
	edge_ids.push_back(graph.addEdge(vertex_ids[6], vertex_ids[2], 12)); //8

	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[0]), graph.getEdge(edge_ids[0]).lock()->getWeight())); //0 15
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[1]), graph.getEdge(edge_ids[1]).lock()->getWeight())); //1 1
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[2]), graph.getEdge(edge_ids[2]).lock()->getWeight())); //2 10
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[3]), graph.getEdge(edge_ids[3]).lock()->getWeight())); //3 13
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[4]), graph.getEdge(edge_ids[4]).lock()->getWeight())); //4 17
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[5]), graph.getEdge(edge_ids[5]).lock()->getWeight())); //5 9
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[6]), graph.getEdge(edge_ids[6]).lock()->getWeight())); //6 8
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[5]), graph.getEdge(edge_ids[5]).lock()->getWeight())); //7 11
	heap_node_ids.push_back(heap.insert(graph.getVertex(vertex_ids[6]), graph.getEdge(edge_ids[6]).lock()->getWeight())); //8 12

	heap.extractMin();

	std::cout << "Should be  8 " << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	auto vertex = graph.getVertex(vertex_ids[0]);
	auto node = heap.find(vertex);
	if (node->getPriority() != 15)
	{
		throw runtime_error{ "Found wrong element!" };
	}

	heap.decreaseKey(vertex, 7); //Decrease Node with weight 15 down to 7;
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be  7 8" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	vertex = graph.getVertex(vertex_ids[2]);
	node = heap.find(vertex);
	if (node->getPriority() != 10)
	{
		throw runtime_error{ "Found wrong element!" };
	}

	heap.decreaseKey(vertex, 5);
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be 5 7 8" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);
}