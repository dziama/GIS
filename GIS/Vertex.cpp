#include "Vertex.h"

Vertex::Vertex(VertexId id) : m_Id{id}
{
}

VertexId Vertex::getId()
{
	return m_Id;
}

void Vertex::addEdge(EdgePtr ptr)
{
	m_Edges.push_back(ptr);
}

vector<EdgeId> Vertex::getEdges()
{
	vector<EdgeId> ids;
	for (auto& edge : m_Edges)
	{
		auto ptr = edge.lock();
		ids.push_back(ptr->getId());
	}
	return ids;
}