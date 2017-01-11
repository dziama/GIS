#pragma once

#include "Interfaces.h"

//Klasa wierzcholka grafu
class Vertex : public IVertex
{
public:
	//Konstruktor
	Vertex(VertexId id);

	//Metody interfejsowe
	virtual VertexId getId() override;
	virtual void addEdge(EdgePtr ptr) override;
	virtual vector<EdgeId> getEdges() override;

private:

	//Numer wierzcholka
	VertexId m_Id;

	//Wskazniki na krawedzie sasiadujace z wierzcholkiem
	vector<EdgePtr> m_Edges;
};

