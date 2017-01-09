#include "HeapNode.h"

HeapNode::HeapNode(VertexPtr vert, HeapNodeId num) :
	m_Degree{0}, m_Next{}, m_Prev{}, 
	m_Marked{false}, m_NodeNumber{num}
{
	if (vert.lock() == nullptr)
	{
		throw exception{ "Heap node cannot contain nullptr to Vertex!" };
	}

	m_Vertex = vert;
	m_Weight = std::numeric_limits<EdgeWeight>::max();
}

HeapNode::HeapNode(VertexPtr vert, HeapNodeId num, EdgeWeight initWeight) : 
	m_Degree{ 0 }, m_Next{}, m_Prev{},
	m_Marked{ false }, m_NodeNumber{ num }
{
	m_Vertex = vert;
	m_Weight = initWeight;
}

void HeapNode::changeWeight(EdgeWeight weight)
{
	if (m_Weight > weight)
	{
		m_Weight = weight;
	}
	else
	{
		throw exception{ "HeapNode: Tried to change weight to higher!" };
	}
}

 HeapNodePtr HeapNode::getParent()
 {
	 return m_Parent;
 }
 
 void HeapNode::setParent(HeapNodePtr ptr)
 {
	 m_Parent = ptr;
 }

 HeapNodePtr HeapNode::getChild()
 {
	 return m_Child;
 }
 
 void HeapNode::setChild(HeapNodePtr ptr)
 {
	 m_Child = ptr;
 }

 NodeDegree HeapNode::getDegree()
 {
	 return m_Degree;
 }
 
 //void HeapNode::setDegree(NodeDegree degree)
 //{
//	 m_Degree = degree;
 //}

 void HeapNode::increaseDegree()
 {
	 ++m_Degree;
 }

 void HeapNode::decreaseDegree()
 {
	 --m_Degree;
 }

 void HeapNode::setNext(HeapNodePtr ptr)
 {
	 m_Next = ptr;
 }
 
 HeapNodePtr HeapNode::getNext()
 {
	 return m_Next;
 }

 void HeapNode::setPrev(HeapNodePtr ptr)
 {
	 m_Prev = ptr;
 }
 
 HeapNodePtr HeapNode::getPrev()
 {
	 return m_Prev;
 }

 VertexPtr HeapNode::getVertex()
 {
	 return m_Vertex;
 }
 
 bool HeapNode::isMarked()
 {
	 return m_Marked;
 }
 
 void HeapNode::mark()
 {
	 m_Marked = true;
 }

 void HeapNode::unmark()
 {
	 if (m_Marked)
	 {
		m_Marked = false;
	 }
 }

 bool HeapNode::hasChild()
 {
	 return m_Child.lock() != nullptr;
 }
 
 bool HeapNode::hasParent()
 {
	 return m_Parent.lock() != nullptr;
 }

 EdgeWeight HeapNode::getPriority()
 {
	 return m_Weight;
 }

 HeapNodeId HeapNode::getNodeNumber()
 {
	 return m_NodeNumber;
 }