// GIS_tests.cpp : Defines the entry point for the console application.
//
#include "FibbonacciHeapTestClass.h"
#define BOOST_TEST_MODULE GIS_TESTS
#include <boost\test\included\unit_test.hpp>

string TEST_FILES_PATH = "E:\\";

BOOST_AUTO_TEST_CASE(INSERTING_INTO_ROOT_LIST_TEST)
{
	ifstream graph_ifstream;
	string graph_file = TEST_FILES_PATH + "Graf.txt";

	graph_ifstream.open(graph_file.c_str());

	Matrix weight_matrix{};
	Graph graph{};
	FibonacciHeap test_heap;

	if (graph_ifstream.is_open())
	{
		readMatrix(graph_ifstream, weight_matrix);

		vector<VertexId> vertex_ids;
		vector<EdgeId> edge_ids;

		for (unsigned i = 0; i < weight_matrix.size(); ++i)
		{
			vertex_ids.push_back(graph.addVertex());
		}

		for (unsigned i = 0; i < weight_matrix.size(); ++i)
		{
			for (unsigned j = 0; j < weight_matrix.size(); ++j)
			{
				if (weight_matrix[i][j] == 0L)
				{
					continue;
				}
				else
				{
					graph.addEdge(vertex_ids[i], vertex_ids[j], weight_matrix[i][j]);
				}
			}
		}

		auto& vertices = graph.getVertices();
		auto& edges = graph.getEdges();

		for (unsigned i = 0; i < vertices.size(); ++i)
		{
			test_heap.insert(vertices[i], edges[i]);
			verifyNodeDoubleLinkedList(test_heap.peekMinElement().lock());
		}
	}
	else
	{
		throw exception{ "Test graph file not found!" };
	}
}

BOOST_AUTO_TEST_CASE(HEAP_LINK_TEST)
{
	FibbonacciHeapTestClass test;
	test.testHeapLink();
}

BOOST_AUTO_TEST_CASE(EXTRACT_MIN_TEST)
{
	FibbonacciHeapTestClass test;
	test.testExtractMin();
}

BOOST_AUTO_TEST_CASE(MOVE_NODE_CHILDREN_TEST)
{
	FibbonacciHeapTestClass test;
	test.testMoveNodeChildren();
}

BOOST_AUTO_TEST_CASE(BUILD_AND_EMPTY_SMALLHEAP)
{
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());
	vertex_ids.push_back(graph.addVertex());

	edge_ids.push_back(graph.addEdge(vertex_ids[0], vertex_ids[1], 15));
	edge_ids.push_back(graph.addEdge(vertex_ids[1], vertex_ids[2], 1));
	edge_ids.push_back(graph.addEdge(vertex_ids[2], vertex_ids[3], 10));
	edge_ids.push_back(graph.addEdge(vertex_ids[4], vertex_ids[0], 13));
	edge_ids.push_back(graph.addEdge(vertex_ids[3], vertex_ids[1], 17));

	for (unsigned itr = 0; itr < vertex_ids.size(); ++itr)
	{
		heap.insert(graph.getVertex(vertex_ids[itr]), graph.getEdge(edge_ids[itr]));
		verifyNodeDoubleLinkedList(heap.peekMinElement().lock());
	}

	unsigned itr = 0;

	vector<EdgeWeight> correct_sequence;
	correct_sequence.push_back(1);
	correct_sequence.push_back(10);
	correct_sequence.push_back(13);
	correct_sequence.push_back(15);
	correct_sequence.push_back(17);
	while (heap.isEmpty() == false)
	{
		auto min = heap.extractMin();
		if (min->getPriority() != correct_sequence[itr])
		{
			throw exception{ "Invalid priority node extracted!" };
		}
		itr++;
	}
}

BOOST_AUTO_TEST_CASE(LARGE_SEQUENCE_TEST)
{
	/*ifstream seq_ifstream;
	string seq_file = TEST_FILES_PATH + "Test_Seq.txt";

	seq_ifstream.open(seq_file.c_str());

	Graph graph{};
	FibonacciHeap test_heap{};
	vector<GraphEdgePtr> edges{};

	if (seq_ifstream.is_open())
	{
		string line;
		stringstream ss;
		getline(seq_ifstream, line);

		ss << line;
		EdgeWeight weight;
		EdgeId edge_id = 0;
		unsigned edge_itr = 0;
		do
		{
			ss >> weight;
			auto id =graph.addVertex();
			edges.push_back(GraphEdgePtr{ new Edge{edge_id++, weight} });
			test_heap.insert(graph.getVertex(id), edges[edge_itr]);
			++edge_itr;
		} while (ss.eof() == false);

		EdgeWeight i = 1;
		while (test_heap.isEmpty() == false)
		{
			auto min = test_heap.extractMin();
			if (i != min->getPriority())
			{
				throw exception{ "Sequence invalid!" };
			}
			++i;
		}
	}
	else
	{
		throw exception{ "Sequence file not found!" };
	}*/
}

BOOST_AUTO_TEST_CASE(THIRD_DEGREE_SUBHEAP_TEST)
{
	Graph graph{};
	FibonacciHeap heap{};
	vector<VertexId> vertex_ids{};
	vector<EdgeWeight> edge_ids{};
	vector<HeapNodeId> heap_node_ids{};

	long graph_matrix[10][10] =
	{
		{0,1,4,3,0,5,6,0,9,8},
		{1,0,7,6,0,2,3,1,5,0},
		{4,7,0,9,2,3,1,0,0,0},
		{3,6,9,0,7,2,0,0,1,2},
		{0,0,2,7,0,4,4,3,8,5},
		{5,2,3,2,4,0,0,0,1,2},
		{6,3,1,0,4,0,0,6,9,3},
		{0,1,0,0,3,0,6,0,7,8},
		{9,5,0,1,8,1,9,7,0,1},
		{8,0,0,2,5,2,3,8,1,0}
	};

	for (unsigned i = 0; i < 10; ++i)
	{
		vertex_ids.push_back(graph.addVertex());
	}

	for (unsigned i = 0; i < 10; ++i)
	{
		for (unsigned j = 0; j < 10; ++j)
		{
			if (graph_matrix[i][j] == 0L)
			{
				continue;
			}
			else if (i == j)
			{
				continue;
			}
			else
			{
				EdgeId id = graph.addEdge(vertex_ids[i], vertex_ids[j], graph_matrix[i][j]);
				if (id != -1)
				{
					edge_ids.push_back(id);
				}
			}
		}
	}

	for (unsigned i = 0; i < 10; ++i)
	{
		heap.insert(graph.getVertex(vertex_ids[i]), graph.getEdge(edge_ids[i]));
	}

	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());
	heap.printNodeList(std::cout, heap.peekMinElement());

	auto min = heap.extractMin();
	verifyNodeDoubleLinkedList(heap.peekMinElement().lock());

	std::cout << "Should be 2" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement(), true);

	auto child = heap.peekMinElement().lock()->getNext().lock()->getChild();
	verifyNodeDoubleLinkedList(child.lock());

	std::cout << "Should be 6 4 5" << endl;
	heap.printNodeList(std::cout, child);
}