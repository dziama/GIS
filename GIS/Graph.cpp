#include "Graph.h"

Graph::Graph()
{
	m_NextVertexId = 0;
	m_NextEdgeId = 0;
}

VertexId Graph::addVertex()
{
	VertexId used_id = m_NextVertexId++;
	GraphVertexPtr ptr(new Vertex(used_id));
	m_Vertices.insert(pair<VertexId, GraphVertexPtr>(used_id, ptr));
	return used_id;
}

EdgeId Graph::addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight)
{
	bool edge_exists = false;
	if (m_Vertices.count(vertexId1) == 1 && m_Vertices.count(vertexId2) == 1)
	{
		auto edges_ids = m_Vertices[vertexId1]->getEdges();
		
		for (auto& id : edges_ids)
		{
			auto first = m_Edges[id]->getFirstVertex().lock();
			auto second = m_Edges[id]->getSecondVertex().lock();

			if (first->getId() == vertexId2 && second->getId() == vertexId1)
			{
				edge_exists = true;
				break;
			}
			else if (first->getId() == vertexId1 && second->getId() == vertexId2)
			{
				edge_exists = true;
				break;
			}
			else
			{
				continue;
			}
		}

		if (edge_exists)
		{
			//throw exe? trying to add existing edge?
		}
		else
		{
			EdgeId id = m_NextEdgeId++;
			GraphEdgePtr ptr(new Edge(id, weight));
			ptr->setFirstVertex(VertexPtr(m_Vertices[vertexId1]));
			ptr->setSecondVertex(VertexPtr(m_Vertices[vertexId2]));
			m_Edges.insert(pair<EdgeId, GraphEdgePtr>(id, ptr));
		}


	}
	else
	{
		return -1;
	}
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