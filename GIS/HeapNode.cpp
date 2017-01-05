#include "HeapNode.h"

HeapNode::HeapNode(VertexPtr vert, EdgePtr edge, HeapNodeId num) :
	m_Degree{0}, m_Next{}, m_Prev{}, 
	m_Marked{false}, m_NodeNumber{num}
{
	if (vert.lock() == nullptr)
	{
		throw exception{ "Heap node cannot contain nullptr to Vertex!" };
	}

	if (edge.lock() == nullptr)
	{
		throw exception{ "Heap node cannot contain nullptr to Edge!" };
	}

	m_Vertex = vert;
	m_Edge = edge;
}

//HeapNode::HeapNode(VertexPtr ptr, EdgeWeight priority, HeapNodePtr next, HeapNodePtr prev) :
//	m_Vertex{ ptr }, m_Degree{ 0 }, m_Next{ next }, m_Prev{ prev }, m_Priority{ priority }, m_Child{}, m_Parent{}, m_Marked{ false }
//{
//
//}

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

 void HeapNode::increaseDegree(NodeDegree increase)
 {
	 m_Degree += increase;
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
	 return m_Edge.lock()->getWeight();
 }

 HeapNodeId HeapNode::getNodeNumber()
 {
	 return m_NodeNumber;
 }