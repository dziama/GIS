#include "Vertex.h"

Vertex::Vertex(VertexId id) : m_Id{id}
{
}

VertexId Vertex::getId()
{
	return 0L;
}

void Vertex::addEdge(EdgePtr ptr)
{
	return 0L;
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