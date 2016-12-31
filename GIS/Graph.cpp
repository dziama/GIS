#include "Graph.h"

Graph::Graph()
{
	m_NextVertexId = 1;
	m_NextEdgeId = 1;
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
			if (m_Edges.count(id) == 1)
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
			else
			{
				continue;
			}
		}

		if (edge_exists)
		{
			return -1;
			//throw exe? trying to add existing edge?
		}
		else
		{
			EdgeId id = m_NextEdgeId++;
			GraphEdgePtr ptr(new Edge(id, weight));
			ptr->setFirstVertex(VertexPtr(m_Vertices[vertexId1]));
			ptr->setSecondVertex(VertexPtr(m_Vertices[vertexId2]));
			m_Edges.insert(pair<EdgeId, GraphEdgePtr>(id, ptr));
			m_Vertices[vertexId1]->addEdge(m_Edges[id]);
			m_Vertices[vertexId2]->addEdge(m_Edges[id]);
			return id;
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

void Graph::printEdges(ostream& stream)
{
	stream << "Printing graph edges:" << endl;
	for (auto& edge : m_Edges)
	{
		stream << "Edge id:" << edge.first
			<< "  First vertex id: " << edge.second->getFirstVertex().lock()->getId()
			<< "  Second vertex id: " << edge.second->getSecondVertex().lock()->getId()
			<< "  Weight: " << edge.second->getWeight()
			<< endl;
	}
	stream << "-----------------------" << endl;
}

void Graph::printVertices(ostream& stream)
{
	stream << "Printing graph vertices:" << endl;
	for (auto& vertex : m_Vertices)
	{
		stream << "Vertex id:" << vertex.first << "Connected edges: ";
		auto edges = vertex.second->getEdges();
		for (auto& edgeid : edges)
		{
			stream << edgeid << " ";
		}

		stream << endl;
	}
}

Vertices Graph::getVertices()
{
	return m_Vertices;
}

Edges Graph::getEdges()
{
	return m_Edges;
}