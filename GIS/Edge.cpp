#include "Edge.h"

Edge::Edge(EdgeId id, EdgeWeight weight) : m_Id{id}, m_Weight{weight}
{
}

EdgeId Edge::getId()
{
	return 0L;
}

EdgeWeight Edge::getWeight()
{
	return 0L;
}

void Edge::setFirstVertex(VertexPtr ptr)
{

}

void Edge::setSecondVertex(VertexPtr ptr)
{

}

VertexPtr Edge::getFirstVertex()
{

}

VertexPtr Edge::getSecondVertex()
{

}