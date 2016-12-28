#pragma once

#include "Interfaces.h"

class Graph : public IGraph
{
public:
	Graph();

	virtual VertexId addVertex() override;
	virtual EdgeId addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight) override;

	virtual VertexPtr getVertex(VertexId vertexId) override;
	virtual EdgePtr getEdge(EdgeId edgeId) override;

	virtual void getMinimumSpanningTree(GraphPtr& ptr) override;
};

