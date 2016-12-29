#include "Edge.h"

Edge::Edge(EdgeId id, EdgeWeight weight) : m_Id{id}, m_Weight{weight}
{
}

EdgeId Edge::getId()
{
	return m_Id;
}

EdgeWeight Edge::getWeight()
{
	return m_Weight;
}

void Edge::setFirstVertex(VertexPtr ptr)
{
	m_FirstVertex = ptr;
}

void Edge::setSecondVertex(VertexPtr ptr)
{
	m_SecondVertex = ptr;
}

VertexPtr Edge::getFirstVertex()
{
	return m_FirstVertex;
}

VertexPtr Edge::getSecondVertex()
{
	return m_SecondVertex;
}