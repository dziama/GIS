#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

using std::weak_ptr;
using std::unique_ptr;
using std::shared_ptr;
using std::map;
using std::vector;
using std::string;
using std::make_shared;
using std::move;
using std::pair;
using std::ostream;
using std::endl;


class IVertex;
class IEdge;
class IGraph;
class IGraphNode;

typedef long VertexId;
typedef long EdgeId;
typedef long VertexPriority;
typedef long EdgeWeight;
typedef int NodeDegree;

typedef weak_ptr<IVertex> VertexPtr;
typedef unique_ptr<IGraph> GraphPtr;
typedef weak_ptr<IEdge> EdgePtr;
typedef weak_ptr<IGraphNode> GraphNodePtr;

class IVertex
{
public:
	virtual VertexId getId() = 0;
	virtual void addEdge(EdgePtr ptr) = 0;
	virtual vector<EdgeId> getEdges() = 0;
	//virtual void deleteEdge(EdgeId edgeId) = 0;
};

class IEdge
{
public:
	virtual EdgeId getId() = 0;
	//virtual void setId(EdgeId id) = 0;

	virtual EdgeWeight getWeight() = 0;
	//virtual void setWeight(EdgeWeight priority) = 0;

	virtual void setFirstVertex(VertexPtr ptr) = 0;
	virtual void setSecondVertex(VertexPtr ptr) = 0;
	virtual VertexPtr getFirstVertex() = 0;
	virtual VertexPtr getSecondVertex() = 0;
};

class IGraphNode
{
public:
	virtual GraphNodePtr getParent() = 0;
	virtual void setParent(GraphNodePtr ptr) = 0;

	virtual GraphNodePtr getChild() = 0;
	virtual void setChild(GraphNodePtr ptr) = 0;

	virtual NodeDegree getDegree() = 0;
	virtual void setDegree(NodeDegree degree) = 0;

	virtual void setNext(GraphNodePtr ptr) = 0;
	virtual GraphNodePtr getNext() = 0;

	virtual void setPrev(GraphNodePtr ptr) = 0;
	virtual GraphNodePtr getPrev() = 0;

	virtual VertexPtr getVertex() = 0;
	virtual bool isMarked() = 0;
	virtual void mark() = 0;

	virtual bool hasChild() = 0;
	virtual bool hasParent() = 0;
};

class IGraph
{
public:
	virtual VertexId addVertex() = 0;
	virtual EdgeId addEdge(VertexId vertexId1, VertexId vertexId2, EdgeWeight weight) = 0;

	virtual VertexPtr getVertex(VertexId vertexId) = 0;
	virtual EdgePtr getEdge(EdgeId edgeId) = 0;

	virtual void getMinimumSpanningTree(GraphPtr& ptr) = 0;

	virtual void printEdges(ostream& stream) = 0;
	virtual void printVertices(ostream& stream) = 0;
};