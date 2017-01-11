#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap() : m_NextFreeNodeNumber{0L}
{
}


FibonacciHeap::~FibonacciHeap()
{
	m_MinElement.lock() = nullptr;
	m_HeapNodes.clear();
}

void FibonacciHeap::cut(NodePtr child, NodePtr parent)
{
	//Pobierz nastepny i poprzedni element wezla-dziecka
	auto child_next = child->getNext().lock();
	auto child_prev = child->getPrev().lock();

	//Obsluga przypadku gdy wezel majacy byc odciety, jest jednoczesnie wskazywany przez rodzica
	//jako wezel o najwyzszym stopniu
	if (parent->getChild().lock() == child)
	{
		if (parent->getDegree() == 1)
		{
			parent->setChild(HeapNodePtr{});
		}
		else if (child_next->getDegree() == child->getDegree() - 1)
		{
			parent->setChild(child_next);
		}
		else if(child_prev->getDegree() == child->getDegree() - 1)
		{
			parent->setChild(child_prev);
		}
		else
		{
			parent->setChild(child_prev);
			//throw exception{ "Cut: Error setting new child node of parent when cutting one of its children!" };
		}
	}

	//Usun z listy dzieci rodzica, usun wskanznik na rodzica, odznacz, zmniejsz stopien rodzica
	removeFromList(child);
	child->setParent(HeapNodePtr{});
	parent->decreaseDegree();
	child->unmark();

	//wstaw odciete dziecko do listy korzeni kopca
	insertIntoRootList(child);
}

void FibonacciHeap::cascadeCut(NodePtr node)
{
	auto node_parent = node->getParent().lock();

	//Sprawdz czy rodzic wezla nie jest pusty
	if (node_parent != nullptr)
	{
		//Jesli jest nieoznaczony, to oznacz
		if (node_parent->isMarked() == false)
		{
			node_parent->mark();
		}
		else
		{
			//Jesli jest oznaczony, kaskadowo odetnij go
			cut(node, node_parent);
			cascadeCut(node_parent);
		}
	}
}

NodePtr FibonacciHeap::find(VertexPtr ptr)
{
	NodePtr node_ptr{};
	auto id = ptr.lock()->getId();
	
	for (auto& heapNode : m_HeapNodes)
	{
		auto vertex = heapNode.second->getVertex().lock();
		if (vertex->getId() == id)
		{
			node_ptr = heapNode.second;
		}
	}
	
	return node_ptr;
}

NodePtr FibonacciHeap::find(HeapNodeId id)
{
	NodePtr node_ptr{};

	if (m_HeapNodes.count(id) == 1)
	{
		node_ptr = m_HeapNodes[id];
	}

	return node_ptr;
}


void FibonacciHeap::decreaseKey(VertexPtr ptr, EdgeWeight weight)
{
	//Znajdz wezel i pobierz jego ojca
	auto node = find(ptr);
	auto node_parent = node->getParent().lock();

	//Nie chcemy powiekszyc wartosci..., chcemy ja zmniejszyc
	if (node->getPriority() < weight)
	{
		throw exception{ "Edge to be changed has higher weight than previous!" };
	}

	//Zmieniamy priorytet
	node->setPriority(weight);

	//Sprawdzamy czy nie naruszona zostala zasada kopca: wartosc priorytetu ojca zawsze musi byc mniejsza od dzieci
	if ((node_parent != nullptr) && (node->getPriority() < node_parent->getPriority()))
	{
		//Jesli naruszona zostala, odcinamy dzieci kaskadowo
		cut(node, node_parent);
		cascadeCut(node_parent);
	}

	//Aktualizujemy element minimalny kopca
	if (node->getPriority() < m_MinElement.lock()->getPriority())
	{
		m_MinElement = node;
	}
}

HeapNodeId FibonacciHeap::insert(VertexPtr ptr, EdgeWeight weight)
{
	//Postych wskaznikow na wezel nie chcemy
	if (ptr.lock() == nullptr)
	{
		throw exception{ "FibbonacciHeap: VertexPtr passed as nullptr!" };
	}

	HeapNodeId id = m_NextFreeNodeNumber++;
	NodePtr new_node{};

	//Tworzymy nowy wezel
	new_node = NodePtr(new HeapNode{ ptr, id, weight });
		
	//Wstawiamy wezel
	m_HeapNodes.insert(HeapPair(new_node->getNodeNumber(), new_node));
	new_node = m_HeapNodes[new_node->getNodeNumber()];

	//Ustawiamy ze na razie jest jedyny na liscie (wskazuje na sam siebie)
	new_node->setNext(new_node);
	new_node->setPrev(new_node);

	//Wstawiamy do wlasciwej listy korzeni kopca, trafi na lewo(wskazik Prev) od elementu minimalnego
	insertIntoRootList(new_node);
	
	//Zwracamy id nowego wezla
	return id;
}

bool FibonacciHeap::isEmpty()
{
	return pointerEmpty(m_MinElement);
}

bool FibonacciHeap::pointerEmpty(const HeapNodePtr& ptr)
{
	if (ptr.use_count() == 0L)
	{
		return true;
	}
	else
	{
		return false;
	}
}

NodePtr FibonacciHeap::extractMin()
{
	//Sprawdzamy czy kopiec przypadkiem nie jest pusty, jesli tak, wskaznik na element minimalny bedzie pusty
	NodePtr ptr;
	if (m_MinElement.lock() != nullptr)
	{
		//Pobierz element minimalny, wywolaj pieklo... tych wszystkich operacji budujacych kopiec
		ptr = removeMinFromRootList();
		m_HeapNodes.erase(ptr->getNodeNumber());
	}

	return ptr;
}

void FibonacciHeap::removeFromList(NodePtr& ptr)
{
	//Pobierz element nastepny i poprzedni
	auto next = ptr->getNext().lock();
	auto prev = ptr->getPrev().lock();

	//poprzedni teraz wskazuje na nastepny i vice wersa, w obie strony
	next->setPrev(prev);
	prev->setNext(next);

	//Wyjety element wskazuje na sam siebie w obie strony
	ptr->setNext(ptr);
	ptr->setPrev(ptr);
}

void FibonacciHeap::heapLink(NodePtr& toChild, NodePtr& toParent)
{
	//Wyjmij wezel-dziecko z listy w ktorej sie znajduje
	removeFromList(toChild);

	//Pobierz jego stopien
	NodeDegree child_degree = toChild->getDegree();

	//Procedura dolaczania wezla-dziecka do wezla-rodzica o wyrzszym stopniu niz 1
	if (toParent->getDegree() > 1)
	{
		//Wezel dziecko zostanie wstawiony miedzy te dwa wezly
		auto child = toParent->getChild().lock();
		auto child_next = child->getNext().lock();

		toChild->setPrev(child);
		child->setNext(toChild); 

		toChild->setNext(child_next);
		child_next->setPrev(toChild);

		//Zmiana wskazania wezla-rodzica, musi zawsze wskazywac dziecko o najwyrzszym stopniu
		if (child_degree > child->getDegree())
		{
			toParent->setChild(toChild);
		}
	}
	else if (toParent->getDegree() == 1)
	{
		//wezel-rodzic ma tylko jedno dziecko..
		auto child = toParent->getChild().lock();

		//Te dwa wezly wskazuja obydowoma swoimi wskaznikami jeden drugiego, tak to jest pomyslane...
		toChild->setPrev(child);
		child->setNext(toChild);

		toChild->setNext(child);
		child->setPrev(toChild);

		//Ewentualnie aktualizujemy glowny wezel-dziecko wezla-rodzica
		if (child_degree > child->getDegree())
		{
			toParent->setChild(toChild);
		}
	}
	else
	{
		//Jesli wezel-rodzic jest stopnia 0, sprawa jest prosta, dziecko wskazuje na samo siebie
		toChild->setNext(toChild);
		toChild->setPrev(toChild);

		//a rodzic wskazuje na nowe dziecko
		toParent->setChild(toChild);
	}

	//W kazdym przyoadku, stopnien rodzica rosnie o jeden, dziecko aktualizuje wskaznik na rodzica i jest odznaczane
	toParent->increaseDegree();
	toChild->setParent(toParent);
	toChild->unmark();
}

void FibonacciHeap::consolidate(NodePtr begin)
{
	//Pomocnicza tabela i wszystkie wezly ktore trzeba odwiedzic
	vector<NodePtr> aux_table;
	vector<NodePtr> nodes_to_visit;
	
	//Pobierz wszystkie wezly z listy korzeni kopca
	auto current_node = begin;
	do
	{
		nodes_to_visit.push_back(current_node);
		current_node = current_node->getNext().lock();
	} while (current_node != begin);

	NodeDegree degree = 0;

	//Dynamiczne zwiekszanie pomocniczej tabeli, ochrona przed wyjsciem poza indeks
	auto& auxTableSentry = [&aux_table](NodeDegree d) -> bool
	{
		if (aux_table.size() == 0)
		{
			aux_table.push_back(nullptr);
		}

		if(aux_table.size() - 1 < d)
		{
			auto diff = d - aux_table.size() + 1;
			for (unsigned i = 0; i < diff; ++i)
			{
				aux_table.push_back(nullptr);
			}
		}

		if (aux_table[d] == nullptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	//Dla kazdego wezla ktory ma byc odwiedzony...
	for (auto& visted_node : nodes_to_visit)
	{
		degree = visted_node->getDegree();

		//Zdejmij wezel stopnia zero z listy korzeni
		if (degree == 0)
		{
			removeFromList(visted_node);
		}

		//Dopoki w tablicy indeksowanej stopniami wezlow nie trafimy na nullptr...
		while (auxTableSentry(degree))
		{
			auto y = aux_table[degree];

			//Upewniamy sie ze dobrze wywolamy heap-link, rodzicem ma byc wezel o wyrzszym priorytecie(nizszej wartosci)
			//Jesli tak nie jest, zamieniamy wezly
			if (visted_node->getPriority() > y->getPriority())
			{
				visted_node.swap(y);
			}

			//Laaczymy wezly, y staje sie dzieckiem visted_node
			heapLink(y, visted_node);
			
			//Polaczylismy np. dwa wezly stopnia 0 w wezel stopnia 1, w tablicy zwalniamy indeks 0
			aux_table[degree] = nullptr;

			//teraz zajmujemy sie stopniem wyrzszym
			++degree;

			//Upewniamy sie ze nasza tablica jest odpowiednio duza...
			auxTableSentry(degree);
		}

		auxTableSentry(degree);

		//Wstawiamy dany wezel na odpowiednie miejsce w tablicy
		aux_table[degree] = visted_node;
	}

	//Po tym wszystkim element minimalny musi byc od nowa ustalony...
	m_MinElement = HeapNodePtr{};

	//Co robi ta petla
	for (auto& node : aux_table)
	{
		if (node != nullptr)
		{
			//Przy okazji wstawia do listy korzeni kopca wszystkie wezly utworzone i przechowane w
			//pomocniczej tablicy
			insertIntoRootList(node);

			//Jesli mamy lepszego kandydata...
			if (node->getPriority() < m_MinElement.lock()->getPriority())
			{
				m_MinElement = node;
			}
		}
	}
}

void FibonacciHeap::insertIntoRootList(NodePtr& node)
{
	//Jelsi kopiec nie jest pusty...
	if (m_MinElement.lock() != nullptr)
	{
		//Pobieramy sam element minimalny i jego poprzednika
		auto min_element = m_MinElement.lock();
		auto min_prev = min_element->getPrev().lock();

		//Ustawiamy wskazniki, nowy element laduje miedzy wezlem minimalnym oraz poprzednikiem tego ostatniego,
		min_element->setPrev(node);
		min_prev->setNext(node);

		node->setNext(min_element);
		node->setPrev(min_prev);

		//Jesli nowo wstawiony element ma wyzszy priorytet(wartosc nizsza) od obecnego minimalnego, to ustawiamy go jako
		//nowy element minimalny
		if (node->getPriority() < min_element->getPriority())
		{
			m_MinElement = node; 
		}
	}
	else
	{
		//Jesli jest, to cokolwiek bysmy nie dali, staje sie nowym elementem minimalnym
		m_MinElement = node;
	}
}

NodePtr FibonacciHeap::moveNodeChildrenToRootList(NodePtr& node)
{
	//Pobieramy wskaznik na nastepny i poprzedni element danego wezla w liscie korzeni kopca
	auto next = node->getNext().lock();
	auto prev = node->getPrev().lock();

	//Nie powinien miec rodzica...
	if (node->hasParent())
	{
		throw exception{ "Node to be extracted cannot have parent!" };
	}

	//Jesli ma dzieci, sprawa sie ciut komplikuje
	if (node->hasChild())
	{
		if (node->getDegree() == 1)
		{
			//Jelsi jest tylko jedno dziecko, dodajemy je do listy korzeni kopca
			NodePtr child = node->getChild().lock();

			child->setParent(HeapNodePtr{});
			child->setNext(next);
			child->setPrev(prev);

			next->setPrev(child);
			prev->setNext(child);
		}
		else
		{
			//Jesli wiecej, to trzeba dodac je wszystkie
			NodePtr first_child = node->getChild().lock();
			NodePtr last_child = first_child->getNext().lock();

			prev->setNext(last_child);
			last_child->setPrev(prev);

			first_child->setNext(next);
			next->setPrev(first_child);

			auto current_child = first_child;
			do
			{
				current_child->setParent(HeapNodePtr{});
				current_child = current_child->getPrev().lock();
			} while (current_child != first_child);
		}
	}
	else
	{
		//Jesli nie, wstarczy przepiac wskazniki elementu poprzedniego i nastepnego
		next->setPrev(prev);
		prev->setNext(next);
	}

	node->setChild(HeapNodePtr{});

	//Ta funkcja jest wywolywana zawsze z argumentem jako element minimalny, tymczasowo ustawiamy wskaznik na element
	//minimalny na jego bylego poprzednika, i tak to sie zmieni przezprocedure consolidate
	m_MinElement = prev;

	return next;
}

NodePtr FibonacciHeap::removeMinFromRootList()
{
	auto min_ptr = m_MinElement.lock();

	//Przepnij dzieci wezla do listy korzeni kopca
	auto node = moveNodeChildrenToRootList(min_ptr);

	//Wykonaj konsolidacje, zbuduj wlasciwa strukture kopca fibbonaciego
	consolidate(node);

	return min_ptr;
} 

void FibonacciHeap::printNodeList(ostream& stream, HeapNodePtr& ptr, bool verbose)
{
	//Drukowanie list cyklicznych, listy korzeni kopca... listy dzieci...
	//Proste (tylko w przod)
	//lub rozrzerzone (w przod i w tyl, dodatkowe informacje)
	NodePtr begin = ptr.lock();
	NodePtr current = begin;

	auto size = getSize();
	if (verbose)
	{
		std::cout << "-----------------------FORWARD----------------------------" << endl;
		do
		{
			stream << " Node: " << current->getNodeNumber()
			<< " Priority: " << current->getPriority()
			<< " Degree: " << current->getDegree()
			<< " Marked: " << current->isMarked()
			<< " Children: " << current->hasChild() << endl;
			current = current->getNext().lock();
		} while (current != begin);

		current = begin;

		std::cout << "-----------------------REVERSE----------------------------" << endl;
		do
		{
			stream << " Node: " << current->getNodeNumber()
				<< " Priority: " << current->getPriority()
				<< " Degree: " << current->getDegree()
				<< " Marked: " << current->isMarked()
				<< " Children: " << current->hasChild() << endl;
			current = current->getPrev().lock();
		} while (current != begin);
	}
	else
	{
		do
		{
			stream << " Node: " << current->getPriority() << "  ";
			current = current->getNext().lock();
		} while (current != begin);
		stream << endl;
	}
}

size_t FibonacciHeap::getSize()
{
	return m_HeapNodes.size();
}

HeapNodePtr FibonacciHeap::peekMinElement()
{
	return m_MinElement;
}

void FibonacciHeap::printNode(ostream& stream, HeapNodePtr& ptr)
{
	auto node = ptr.lock();

	if (node == nullptr)
	{
		return;
	}

	if (node->getDegree() == 0)
	{
		stream << node->getPriority();
	}
	else if (node->getDegree() == 1)
	{
		stream << node->getPriority() << "|";
	}
	else
	{
		auto child = node->getChild().lock();

	}
}