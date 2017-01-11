#pragma once
#include "Interfaces.h"

//Klasa wezla kopca fibbonaciego
class HeapNode : public IHeapNode
{
public:

	//Funkcje interfejsowe...
	virtual HeapNodePtr getParent() override;
	virtual void setParent(HeapNodePtr ptr) override;

	virtual HeapNodePtr getChild() override;
	virtual void setChild(HeapNodePtr ptr) override;

	virtual NodeDegree getDegree() override;
	virtual void increaseDegree() override;
	virtual void decreaseDegree() override;

	virtual void setNext(HeapNodePtr ptr) override;
	virtual HeapNodePtr getNext() override;

	virtual void setPrev(HeapNodePtr ptr) override;
	virtual HeapNodePtr getPrev() override;

	virtual VertexPtr getVertex() override;
	virtual bool isMarked() override;
	virtual void mark() override;
	virtual void unmark() override;

	virtual bool hasChild() override;
	virtual bool hasParent() override;

	virtual void setPriority(EdgeWeight weight) override;

	virtual HeapNodeId getNodeNumber() override;

	virtual EdgeWeight getPriority();

	//Konstruktory
	HeapNode(VertexPtr vert, HeapNodeId num);
	HeapNode(VertexPtr vert, HeapNodeId num, EdgeWeight initWeight);

private:

	//Wskazniki na: rodzica, dziecko o najwyrzszym stopniu, wezel nastepny i poprzedni
	HeapNodePtr m_Parent;
	HeapNodePtr m_Child;
	HeapNodePtr m_Next;
	HeapNodePtr m_Prev;

	//Przechowywany wskaznik na wierzcholek
	VertexPtr m_Vertex;

	//Numer wezla
	HeapNodeId m_NodeNumber;

	//Oznaczenie
	bool m_Marked;

	//Stopien
	NodeDegree m_Degree;

	//Priorytet/Waga krawedzi (im nizsza wartosc, tym wyrzszy priorytet)
	EdgeWeight m_Weight;
};