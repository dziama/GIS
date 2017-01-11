#pragma once

#include "Edge.h"
#include "Vertex.h"
#include "HeapNode.h"
#include "Utility.h"

class FibonacciHeap;

typedef shared_ptr<FibonacciHeap> HeapPtr;

//Glowny bohater projektu...
class FibonacciHeap
{
	//Zaprzyjazniona klasa testowa
	friend class FibbonacciHeapTestClass;

public:
	//Konstruktor i destruktor
	FibonacciHeap();
	~FibonacciHeap();

	//Podstawowe operacje dla tego kopca: wstawienie elementu, wyjecie tego o najmniejszej wartosci priorytetu(najmniejsza waga krawedzi)
	//Zmniejszenie wartosci priorytetu jednego z wezlow
	HeapNodeId insert(VertexPtr ptr, EdgeWeight weight = 0);
	NodePtr extractMin();
	void decreaseKey(VertexPtr ptr, EdgeWeight weight);

	//Wypisywanie danych wezlow na danym poziomie np. lista korzeni lub lista dzieci jakiegos wezla...
	void printNodeList(ostream& stream, HeapNodePtr& ptr, bool verbose = false);
	void printNode(ostream& stream, HeapNodePtr& ptr);

	//Podejrzenie jaki jest obecny minimalny element
	HeapNodePtr peekMinElement();  

	//Tlumaczyc nie trzeba...
	bool isEmpty();
	size_t getSize();

	//Wyszukiwanie wezla w kopcu, dwa sposoby
	NodePtr find(VertexPtr ptr);
	NodePtr find(HeapNodeId id);

private:

	//Uchwyt dla shared_ptr-ow, zbior przechowywanych wezlow w kopcu
	HeapNodes m_HeapNodes;

	//Wskaznik na element minimalny
	HeapNodePtr m_MinElement;

	//Kolejne wolne id na kolejnego, nowego dodawanego wezla kopca
	HeapNodeId m_NextFreeNodeNumber;
	
	//Operacje Cut i CascadeCut, wykorzystywane przy decreaseDegree
	void cut(NodePtr child, NodePtr parent);
	void cascadeCut(NodePtr node);

	//Operacje HeapLink i Consolidate, wykorzystywane przy extractMin
	void heapLink(NodePtr& toChild, NodePtr& toParent);
	void consolidate(NodePtr begin);

	//Sprawdza czy dany wskaznik na element kopca jest pusty
	bool pointerEmpty(const HeapNodePtr& ptr);

	//Wstawia element do listy korzeni kopca
	void insertIntoRootList(NodePtr& node);

	//Odczepia wezel z jego listy (np. lista korzeni kopca lub lista dzieci jakiegos wezla)
	void removeFromList(NodePtr& ptr);

	//Operacja wyjmowania wezla z listy korzeni, aktualizuje wskaznik na element minimalny
	//Ewentualnie przeprowadza operacje dodania dzieci danego wezla do listy korzeni
	NodePtr removeMinFromRootList();

	//Dodaje dzieci danego wezla do listy korzeni, odlacza dzieci od wezla-rodzica
	NodePtr moveNodeChildrenToRootList(NodePtr& node);
};

