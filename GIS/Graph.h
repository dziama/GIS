#pragma once

#include "Vertex.h"
#include "Edge.h"

//Klasa reprezentujaca graf
class Graph : public IGraph
{
public:
	Graph();
	~Graph();

	//Metody mowia same za siebie...
	virtual VertexId addVertex() override;
	virtual EdgeId addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight) override;

	virtual VertexPtr getVertex(VertexId vertexId) override;
	virtual EdgePtr getEdge(EdgeId edgeId) override;

	virtual void printEdges(ostream& stream) override;
	virtual void printVertices(ostream& stream) override;

	Vertices getVertices() override;
	Edges getEdges() override;

private:

	//Wierzcholki i krawedzie
	Vertices m_Vertices;
	Edges m_Edges;

	//Kolejne id dla dodawanych nowych krawedzi/wierzcholkow
	VertexId m_NextVertexId;
	EdgeId m_NextEdgeId;
};

