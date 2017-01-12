#include "Graph.h"

Graph::Graph()
{
	m_NextVertexId = 0;
	m_NextEdgeId = 0;
}

Graph::~Graph()
{
	m_Vertices.clear();
	m_Edges.clear();
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

	//Sprawdz czy podane id wierzcholkow rzeczywiscie istnieja...
	if (m_Vertices.count(vertexId1) == 1 && m_Vertices.count(vertexId2) == 1)
	{
		auto edges_ids = m_Vertices[vertexId1]->getEdges();
		
		//Sprawdz czy krawedz miedzy dwoma wierzcholkami przypadkiem juz nie istnieje...
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
			throw runtime_error{ "Edge already exists!" };
		}
		else
		{
			//Dodaj nowa krawedz i ustaw jej dane
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
		throw runtime_error{ "At least one given vertice does not exists when trying to add new edge!" };
	}
}

VertexPtr Graph::getVertex(VertexId vertexId)
{
	if(m_Vertices.count(vertexId) > 0)
	{
		return m_Vertices[vertexId];
	}
	else
	{
		throw runtime_error{ "Requested vertexID not found in Graph!" };
	}
	
}

EdgePtr Graph::getEdge(EdgeId edgeId)
{
	if (m_Edges.count(edgeId) > 0)
	{
		return m_Edges[edgeId];
	}
	else
	{
		throw runtime_error{ "Requested edgeID not found in Graph!" };
	}
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