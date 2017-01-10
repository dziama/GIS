#include "HeapGui.h"

HeapNodeId HeapGui::m_EmptyTile{-1L};

HeapGui::HeapGui(FibonacciHeap& heap, Graph& graph, Font& font) : m_Heap{heap}, m_Graph{graph}, m_Font{font}
{
	m_Window.create(VideoMode{m_WidthDefault, m_HeightDefault}, "Fibbonacci Heap");
	m_Window.setVisible(false);
	m_Window.setFramerateLimit(30);
	m_NextFreeLinkId = 1;

	auto center = getWindowCenter();
	m_ActiveView.setCenter((float)center.x, (float)center.y);
	m_ActiveView.setSize(m_ViewWidth, m_ViewHeight);

	m_Window.setView(m_ActiveView);

	m_TileSize.x = 30;
	m_TileSize.y = 30;

	m_DelayTime = sf::seconds(3.0f);
}

HeapGui::~HeapGui()
{
}

void HeapGui::drawStep()
{
	processEvents();
	m_Window.clear(Color::White);

	drawHeap();
	m_Window.display();

	std::this_thread::sleep_for(1s);
}

void HeapGui::showGui()
{
	m_Window.setVisible(true);

	//pick first vertex
	//auto vertex = m_Graph.getVertex(0).lock();
	//vector<EdgeId> edges;
	//vector<pair<VertexPtr, EdgePtr>> new_heap_elements;

	VertexPairs mst_tree;
	VertexPairs vertex_vertex_parent; //vertex-vertex parent

	processEvents();
	m_Window.clear(Color::White);

	auto vertices = m_Graph.getVertices();
	VertexPtr u;

	//Set each node parent to null
	for (auto& vertex : vertices)
	{
		vertex_vertex_parent.push_back(VertexPair(vertex.second, VertexPtr{}));
	}

	//insert first vertex (with lowest priority)
	m_Heap.insert(vertices[0], 0);
	drawStep();

	//insert rest of vertices, with unsigned long max value(close enough... Infinity)
	for (unsigned i = 1; i < vertices.size(); ++i)
	{
		m_Heap.insert(vertices[i], std::numeric_limits<EdgeWeight>::max());
		drawStep();
	}

	auto getParent = [&vertex_vertex_parent](VertexPtr ptr) -> VertexPtr
	{
		for (auto& pair : vertex_vertex_parent)
		{
			auto first = pair.first.lock();
			auto second = pair.second.lock();

			if (first == ptr.lock())
			{
				return second;
			}
		}
		throw exception{ "getParent lambda: Parent not found!" };
	};

	auto setParent = [&vertex_vertex_parent](VertexPtr vertex, VertexPtr newParent)
	{
		for (auto& pair : vertex_vertex_parent)
		{
			if (pair.first.lock() == vertex.lock())
			{
				pair.second = newParent;
				return;
			}
		}
	};

	auto getEdges = [](VertexPtr& vert, Graph& graph) -> vector<EdgePtr>
	{
		vector<EdgePtr> edges;
		auto edges_ids = vert.lock()->getEdges();

		for (auto& edge_id : edges_ids)
		{
			edges.push_back(graph.getEdge(edge_id));
		}
		return edges;
	};
	
	while (m_Window.isOpen())
	{
		if(m_Heap.isEmpty() == false)
		{
			u = m_Heap.extractMin()->getVertex();
			drawStep();

			auto parent = getParent(u);
			mst_tree.push_back(VertexPair(u, parent));

			auto edges = getEdges(u, m_Graph);

			for (auto& edge : edges)
			{
				auto edge_ptr = edge.lock();
				auto first = edge_ptr->getFirstVertex().lock();
				auto second = edge_ptr->getSecondVertex().lock();

				if (first == u.lock())
				{
					auto node = m_Heap.find(second);
					if ((node != nullptr) && (edge_ptr->getWeight() < node->getPriority()))
					{
						m_Heap.decreaseKey(node->getVertex(), edge_ptr->getWeight());
						drawStep();

						setParent(node->getVertex(), second);
					}
				}
				else if(second == u.lock())
				{
					auto node = m_Heap.find(first);
					if ((node != nullptr) && (edge_ptr->getWeight() < node->getPriority()))
					{
						m_Heap.decreaseKey(node->getVertex(), edge_ptr->getWeight());
						drawStep();

						setParent(node->getVertex(), first);
					}
				}
				else
				{
					continue;
				}
			}
		}

		////pick vertex-edge pairs from chosen vertex
		//edges = vertex->getEdges();
		//for (auto& edgeid : edges)
		//{
		//	auto edge = m_Graph.getEdge(edgeid).lock();
		//	auto first = edge->getFirstVertex().lock();
		//	auto second = edge->getSecondVertex().lock();

		//	if (first->getId() == vertex->getId())
		//	{
		//		new_heap_elements.push_back(pair<VertexPtr, EdgePtr>{second, edge});
		//	}
		//	else if (second->getId() == vertex->getId())
		//	{
		//		new_heap_elements.push_back(pair<VertexPtr, EdgePtr>{first, edge});
		//	}
		//	else
		//	{
		//		throw exception{ "Invalid edge not connected to vertex that has pointer to it@!" };
		//	}
		//}

		//for (auto& p : new_heap_elements)
		//{
		//	
		//}

		//insert them into heap
		//1) Draw unlinked node somewhere, marked and heap with min node
		//2) Draw marked inserted node and min node
		//3) Just draw heap normally
		//redrawing heap after each step

		//extract min element from heap
		//show how min element children are merged into root list
		//1) Draw heap marking all min node children
		//2) Draw heap without min node, with marked extracted min node children
		//3) Draw heap normally

		//show that all root node are marked
		//1) Mark all root nodes
		//2) Draw empty auxillary table (root nodes nubers)
		//3) Draw heap with 
		//draw auxilary table with links to contained nodes
		//show how nodes are picked and how heap-link works
		//redraw after each step
	}
	
}

void HeapGui::processEvents()
{
	Event ev;
	while (m_Window.pollEvent(ev))
	{
		if (ev.type == Event::Closed)
		{
			m_Window.close();
		}
		if (ev.type == Event::KeyPressed)
		{
			if (ev.key.code == sf::Keyboard::Escape)
			{
				m_Window.close();
			}
		}
	}
}

Vector2u HeapGui::getWindowCenter()
{
	auto windows_size = m_Window.getSize();
	return Vector2u{ windows_size.x / 2, windows_size.y / 2 };
}

void HeapGui::clearMatrix()
{
	for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
	{
		for (unsigned j = 0; j < m_DrawMatrix[i].size(); ++j)
		{
			m_DrawMatrix[i][j] = m_EmptyTile;
		}
	}
}

void HeapGui::writeToMatrix(HeapNodeId val, unsigned x, unsigned y)
{
	if (m_DrawMatrix.size() == 0)
	{
		Row row{};
		row.push_back(m_EmptyTile);
		m_DrawMatrix.push_back(row);
	}

	auto temp = x;
	x = y;
	y = temp;

	if (x > m_DrawMatrix.size() - 1)
	{
		auto diff_x = x - m_DrawMatrix.size() + 1;
		auto row_size = m_DrawMatrix[0].size();

		Row mat_row;
		for (unsigned i = 0; i < row_size; ++i)
		{
			mat_row.push_back(m_EmptyTile);
		}

		for (unsigned i = 0; i < diff_x; ++i)
		{
			m_DrawMatrix.push_back(mat_row);
		}
	}

	if (y > m_DrawMatrix[0].size() - 1)
	{
		auto diff_y = y - m_DrawMatrix[0].size() + 1;
		for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
		{
			for (unsigned j = 0; j < diff_y; ++j)
			{
				m_DrawMatrix[i].push_back(m_EmptyTile);
			}
		}
	}

	m_DrawMatrix[x][y] = val;
}

void HeapGui::registerDLinkSiblings(NodePtr& first, NodePtr& second)
{
	DLinkSiblings link{};
	if (first->getNext().lock() == second && second->getPrev().lock() == first)
	{
		link.first_sibling = first;
		link.second_sibling = second;
		m_DLinksSiblings.push_back(link);
	}
	else if (first->getPrev().lock() == second && second->getNext().lock() == first)
	{
		link.first_sibling = first;
		link.second_sibling = second;
		m_DLinksSiblings.push_back(link);
	}
	else
	{

	}
}

void HeapGui::registerSLinkParentChild(NodePtr& node)
{
	if (node->getParent().lock() != nullptr)
	{
		SLinkChildParent link{};
		link.child = node;
		link.parent = node->getParent();
		m_SLinksChildPar.push_back(link);
	}
}

void HeapGui::registerDLinkParentChild(NodePtr& node)
{
	if (node->getParent().lock() != nullptr)
	{
		DLinkParentChild link{};
		link.child = node;
		link.parent = node->getParent();
		m_DLinksParChild.push_back(link);
	}
}

long HeapGui::addNodeToDrawMatrix(NodePtr node, DrawMatrix& m, unsigned x, unsigned y)
{
	writeToMatrix(node->getNodeNumber(), x, y);

	if (node->getDegree() == 0)
	{
		return 0L;
	}
	else if (node->getDegree() == 1)
	{
		addNodeToDrawMatrix(node->getChild().lock(), m, x, y + 2);
		return 0L;
	}
	/*else if (node->getDegree() == 2)
	{
		auto child = node->getChild().lock();
		auto second_child = child->getPrev().lock();



		if (child->getDegree() == 1)
		{
			registerDLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x + 2, y + 2);
		}
		else if (child->getDegree() == 0)
		{
			registerSLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x, y + 2);
		}
		else
		{

		}

		if (child->getDegree() == 1)
		{
			registerDLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x + 2, y + 2);
		}
		if (child->getDegree() == 0)
		{
			registerSLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x, y + 2);
		}


		return 2L;
	}*/
	/*else if (node->getDegree() == 3)
	{
		auto child = node->getChild().lock();

		if (child->getDegree() == 2)
		{
			registerDLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x + 4, y + 2);
		}
		else
		{
			throw exception{ "DrawMatrix: Child of degree 3 subheap is not 2-degree!" };
		}

		child = child->getPrev().lock();
		if (child->getDegree() == 1)
		{
			registerSLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x + 2, y + 2);
		}
		else
		{
			throw exception{ "DrawMatrix:Second child of degree 3 subheap is not 1-degree!" };
		}

		child = child->getPrev().lock();
		if (child->getDegree() == 0)
		{
			registerSLinkParentChild(child);
			addNodeToDrawMatrix(child, m, x, y + 2);
		}
		else
		{
			throw exception{ "DrawMatrix:Third child of degree 3 subheap is not 0-degree!" };
		}

		return 4L;
	}*/
	else
	{
		y = y + 2;
		long w = 0;
		long val = 0;

		vector<NodePtr> children;
		auto first_child = node->getChild().lock();
		auto last_child = first_child->getNext().lock();

		registerDLinkParentChild(first_child);

		auto current = last_child;
		do
		{
			children.push_back(current);
			registerSLinkParentChild(current);
			registerDLinkSiblings(current, current->getNext().lock());
			current = current->getNext().lock();
		} while (current != first_child);

		for (auto& child : children)
		{
			val = addNodeToDrawMatrix(child, m, x, y) ;
			if (val == 0)
			{
				w += 2;
			}
			else
			{
				w += val;
			}
			x = xOffset(x);
		}

		return w;
	}
}

long HeapGui::xOffset(unsigned x)
{
	return x + 2;
}

void HeapGui::drawHeap()
{
	auto min = m_Heap.peekMinElement().lock();
	auto current = min;
	vector<NodePtr> nodes;

	verifyNodeDoubleLinkedList(min);

	do
	{
		nodes.push_back(current);
		current = current->getNext().lock();
	} while (current != min);

	unsigned x = 0;
	unsigned y = 0;
	long w = 0;

	clearMatrix();

	for (auto& node : nodes)
	{
		w = addNodeToDrawMatrix(node, m_DrawMatrix, x, y);
		if (w == 0)
		{
			x += 2;
		}
		else
		{
			x += w + 2;
		}
	}

	HeapNodeGui node_gui{m_TileSize, m_Font};
	node_gui.setPosition(Vector2f(0, 0));

	for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
	{
		for (unsigned j = 0; j < m_DrawMatrix[i].size(); ++j)
		{
			auto current_position = Vector2f(j * m_TileSize.x, i * m_TileSize.y);
			node_gui.setPosition(current_position);
			//tile.setPosition(current_position);
			if (m_DrawMatrix[i][j] != m_EmptyTile)
			{
				auto node_ptr = m_Heap.find(m_DrawMatrix[i][j]);
				node_gui.setNode(node_ptr);

				node_gui.draw(m_Window, m_Heap);
			}
			else
			{
				RectangleShape tile{};
				tile.setFillColor(Color::Cyan);
				tile.setSize(Vector2f(m_TileSize.x, m_TileSize.y));
				tile.setPosition(current_position);
				m_Window.draw(tile);
			}
		}
	}
}