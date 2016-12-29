#pragma once

#include "Vertex.h"
#include "Edge.h"

typedef shared_ptr<IVertex> GraphVertexPtr;
typedef shared_ptr<IEdge> GraphEdgePtr;
typedef map<VertexId, GraphVertexPtr> Vertices;
typedef map<EdgeId, GraphEdgePtr> Edges;

class Graph : public IGraph
{
public:
	Graph();

	virtual VertexId addVertex() override;
	virtual EdgeId addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight) override;

	virtual VertexPtr getVertex(VertexId vertexId) override;
	virtual EdgePtr getEdge(EdgeId edgeId) override;

	virtual void getMinimumSpanningTree(GraphPtr& ptr) override;

	virtual void printEdges(ostream& stream) override;
	virtual void printVertices(ostream& stream) override;

private:

	Vertices m_Vertices;
	Edges m_Edges;
	VertexId m_NextVertexId;
	EdgeId m_NextEdgeId;
};

