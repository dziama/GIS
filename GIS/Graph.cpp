#include "Graph.h"

Graph::Graph()
{
}

VertexId Graph::addVertex()
{
	return 0L;
}

EdgeId Graph::addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight)
{
	return 0L;
}

VertexPtr Graph::getVertex(VertexId vertexId)
{
	return VertexPtr();
}

EdgePtr Graph::getEdge(EdgeId edgeId)
{
	return EdgePtr();
}

void Graph::getMinimumSpanningTree(GraphPtr& ptr)
{

}