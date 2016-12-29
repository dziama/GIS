#include "GraphNode.h"

 GraphNodePtr GraphNode::getParent()
 {
	 return GraphNodePtr();
 }
 
 void GraphNode::setParent(GraphNodePtr ptr)
 {
	 
 }

 GraphNodePtr GraphNode::getChild()
 {
	 return GraphNodePtr();
 }
 
 void GraphNode::setChild(GraphNodePtr ptr)
 {
	 
 }

 NodeDegree GraphNode::getDegree()
 {
	 return 0;
 }
 
 void GraphNode::setDegree(NodeDegree degree)
 {
	 
 }

 void GraphNode::setNext(GraphNodePtr ptr)
 {
	 
 }
 
 GraphNodePtr GraphNode::getNext()
 {
	 return GraphNodePtr();
 }

 void GraphNode::setPrev(GraphNodePtr ptr)
 {
	 
 }
 
 GraphNodePtr GraphNode::getPrev()
 {
	 return GraphNodePtr();
 }

 VertexPtr GraphNode::getVertex()
 {
	 return VertexPtr();
 }
 
 bool GraphNode::isMarked()
 {
	 return false;
 }
 
 void GraphNode::mark()
 {
	 
 }

 bool GraphNode::hasChild()
 {
	 return false;
 }
 
 bool GraphNode::hasParent()
 {
	 return false;
 }