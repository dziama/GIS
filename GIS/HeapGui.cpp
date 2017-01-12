#include "HeapGui.h"

HeapNodeId HeapGui::m_EmptyTile{-1L};

HeapGui::HeapGui(FibonacciHeap& heap, Graph& graph, Font& font) : m_Heap{heap}, m_Graph{graph}, m_Font{font}
{
	//Wstepne ustawienia zmiennych
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
}

void HeapGui::drawStep()
{
	//Zareaguj na zdarzenia
	processEvents();

	//Wczysc okno
	m_Window.clear(Color::White);

	//Narysuj kopiec
	drawHeap();

	//Wyswietl zaktualizowane okno
	m_Window.display();

	//Zaczekaj by widac bylo zmiany
	std::this_thread::sleep_for(2s);
}

void HeapGui::showGui()
{
	//Okno - widoczne
	m_Window.setVisible(true);

	//Budowane drzewo MST
	VertexPairs mst_tree;

	//Zbior par wierzcholkow: wierzcholek i jego rodzic
	VertexPairs vertex_vertex_parent;

	//Wyczysc okno
	m_Window.clear(Color::White);

	//Pobierz wszystkie wierzcholki grafu
	auto vertices = m_Graph.getVertices();

	//Wierzcholek 'u'
	VertexPtr u;

	//Stworz zbior wierzcholkow: wierzcholek : rodzic, zainicjalizuj rodzic�w jako pustych
	for (auto& vertex : vertices)
	{
		vertex_vertex_parent.push_back(VertexPair(vertex.second, VertexPtr{}));
	}

	//Wstaw pierwszy wierzcholek do kopca, z waga 0 (aby byl wyjety jako pierwszy)
	m_Heap.insert(vertices[0], 0);

	//Narysuj
	drawStep();

	//Wstaw pozostale wierzcholki z priorytetem/waga nieskonczonosc (Maxymalna wartosc dla long)
	for (unsigned i = 1; i < vertices.size(); ++i)
	{
		m_Heap.insert(vertices[i], std::numeric_limits<EdgeWeight>::max());
		drawStep();
	}

	//Funkcja Lambda, zwraca rodzica wierzcholka
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
		throw runtime_error{ "getParent lambda: Parent not found!" };
	};

	//Funkcja Lambda, ustawia rodzica wierzcholka
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

	//Funkcja Lambda, zwraca krawedzie sasiednie z podanym wierzcholkiem
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
	
	//Dopoki okno jest aktywne...
	while (m_Window.isOpen())
	{
		//Jesli kopiec nie jest pusty, kontyynujuemy algorytm Prima
		if(m_Heap.isEmpty() == false)
		{
			//Pobierz kolejny wierzcholek o najmniejszej wadze/priorytecie (najlzejsza krawedz)
			u = m_Heap.extractMin()->getVertex();
			drawStep();

			//Pobierz rodzica, wstaw do drzewa MST pare
			auto parent = getParent(u);
			mst_tree.push_back(VertexPair(u, parent));

			//Pobierz krawedzie wierzcholka 'u'
			auto edges = getEdges(u, m_Graph);

			//Dla wszystkich krawedzi wierzcholka 'u'...
			for (auto& edge : edges)
			{
				auto edge_ptr = edge.lock();

				//Pobierz z krawedzi wskazniki na pierwszy i drugi wierzcholek do niej podlaczony
				auto first = edge_ptr->getFirstVertex().lock();
				auto second = edge_ptr->getSecondVertex().lock();


				if (first == u.lock()) //jesli pierwszym jest 'u', zajmij sie drugim
				{
					//Znajdz w kopcu dany wierzcholek
					auto node = m_Heap.find(second);

					//Jesli dany wierzcholek jest w kopcu (nie jest wskaznikiem nullptr) i jego priorytet
					//jest lepszy od tego zapisanego w kopcu...
					if ((node != nullptr) && (edge_ptr->getWeight() < node->getPriority()))
					{
						//Zmniejsz jego priorytet
						m_Heap.decreaseKey(node->getVertex(), edge_ptr->getWeight());
						drawStep();

						//Ustaw nowego rodzica
						setParent(node->getVertex(), second);
					}
				}
				else if(second == u.lock()) //odwrotnie, zajmij sie pierwszym ktory nie jest 'u'
				{
					//Dzialania zupelnie analogicznie jak wyzej
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
			//Przycisk escape konczy dzialanie programu
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
	//Jesli macierz jest pusta, inicjalizujemy ja
	if (m_DrawMatrix.size() == 0)
	{
		Row row{};
		row.push_back(m_EmptyTile);
		m_DrawMatrix.push_back(row);
	}

	//Koordynaty macierzy sa inne niz orientacja na ekranie, zamieniamy wspolzedne
	auto temp = x;
	x = y;
	y = temp;


	//Koordynat x poza wymiarami, zwiekszamy rozmiar macierzy aby sie dopasowac
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

	//Koordynat y poza wymiarami, zwiekszamy rozmiar macierzy aby sie dopasowac
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

	//Bezpiecznie zapisujemy nowa wartosc
	m_DrawMatrix[x][y] = val;
}

void HeapGui::registerDLinkSiblings(const NodePtr& first, const NodePtr& second)
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

void HeapGui::registerSLinkParentChild(const NodePtr& node)
{
	if (node->getParent().lock() != nullptr)
	{
		SLinkChildParent link{};
		link.child = node;
		link.parent = node->getParent();
		m_SLinksChildPar.push_back(link);
	}
}

void HeapGui::registerDLinkParentChild(const NodePtr& node)
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
	//Zapisz dany wezel do macierzy
	writeToMatrix(node->getNodeNumber(), x, y);

	//Wywolaj rekurencyjnie dla jego ewentualnych dzieci
	if (node->getDegree() == 0)
	{
		return 0L;
	}
	else if (node->getDegree() == 1)
	{
		addNodeToDrawMatrix(node->getChild().lock(), m, x, y + 2);
		return 0L;
	}
	else
	{
		y = y + 2;
		long w = 0;
		long val = 0;

		vector<NodePtr> children;
		auto first_child = node->getChild().lock();
		auto last_child = first_child->getNext().lock();

		registerDLinkParentChild(first_child);

		//Pobierz wszystkie dzieci, zarejestruj lacza
		auto current = last_child;
		do
		{
			children.push_back(current);
			registerSLinkParentChild(current);
			registerDLinkSiblings(current, current->getNext().lock());
			current = current->getNext().lock();
		} while (current != first_child);

		//Dla kazdego z dzieci wywolaj metode rekurencyjnie, dostrajajac wspolzedne
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

	//pobierz wszystkie wezly z cyklicznej listy korzeni
	do
	{
		nodes.push_back(current);
		current = current->getNext().lock();
	} while (current != min);

	unsigned x = 0;
	unsigned y = 0;
	long w = 0;

	//Czysc macierz rysowania
	clearMatrix();

	//Dla kazdego wezla na liscie korzeni wywouj rekurencyjnie addNodeToDrawMatrix
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

	//Narysuj wezly kopca korzystajac z macierzy rysowania, skorzystaj z HeapNodeGui
	HeapNodeGui node_gui{m_TileSize, m_Font};
	node_gui.setPosition(Vector2f(0, 0));

	for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
	{
		for (unsigned j = 0; j < m_DrawMatrix[i].size(); ++j)
		{
			//Ustaw pozycje
			auto current_position = Vector2f(j * m_TileSize.x, i * m_TileSize.y);
			node_gui.setPosition(current_position);

			//Jesli dana pozycja w macierzy rysowania nie jest "pusta"
			if (m_DrawMatrix[i][j] != m_EmptyTile)
			{
				auto node_ptr = m_Heap.find(m_DrawMatrix[i][j]);
				node_gui.setNode(node_ptr);

				//Narysuj dany wezel
				node_gui.draw(m_Window, m_Heap);
			}
			else
			{
				//Narysuj "puste" pole
				RectangleShape tile{};
				tile.setFillColor(Color::Cyan);
				tile.setSize(Vector2f(m_TileSize.x, m_TileSize.y));
				tile.setPosition(current_position);
				m_Window.draw(tile);
			}
		}
	}
}