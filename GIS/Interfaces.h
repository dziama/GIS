#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <exception>

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
using std::array;
using std::exception;
using std::stringstream;
using std::ifstream;
using std::getline;

class IVertex;
class IEdge;
class IGraph;
class IHeapNode;

typedef long VertexId;
typedef long EdgeId;
typedef long VertexPriority;
typedef unsigned long EdgeWeight;
typedef unsigned NodeDegree;
typedef long HeapNodeId;
typedef long RootNodeCounter;
typedef long LinkId;

typedef vector<vector<EdgeWeight>> Matrix;

typedef weak_ptr<IVertex> VertexPtr;
typedef unique_ptr<IGraph> GraphPtr;
typedef weak_ptr<IEdge> EdgePtr;


typedef shared_ptr<IVertex> GraphVertexPtr;
typedef shared_ptr<IEdge> GraphEdgePtr;
typedef map<VertexId, GraphVertexPtr> Vertices;
typedef map<EdgeId, GraphEdgePtr> Edges;

typedef shared_ptr<IHeapNode> NodePtr;
typedef weak_ptr<IHeapNode> HeapNodePtr;

typedef pair<HeapNodeId, NodePtr> HeapPair;
typedef map<HeapNodeId, NodePtr> HeapNodes;

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

class IHeapNode
{
public:
	virtual HeapNodePtr getParent() = 0;
	virtual void setParent(HeapNodePtr ptr) = 0;

	virtual HeapNodePtr getChild() = 0;
	virtual void setChild(HeapNodePtr ptr) = 0;

	virtual NodeDegree getDegree() = 0;
	//virtual void setDegree(NodeDegree degree) = 0;
	virtual void increaseDegree(NodeDegree increase) = 0;

	virtual void setNext(HeapNodePtr ptr) = 0;
	virtual HeapNodePtr getNext() = 0;

	virtual void setPrev(HeapNodePtr ptr) = 0;
	virtual HeapNodePtr getPrev() = 0;

	virtual VertexPtr getVertex() = 0;
	virtual bool isMarked() = 0;
	virtual void mark() = 0;
	virtual void unmark() = 0;

	virtual bool hasChild() = 0;
	virtual bool hasParent() = 0;

	virtual HeapNodeId getNodeNumber() = 0;

	virtual EdgeWeight getPriority() = 0;
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

	virtual Vertices getVertices() = 0;
	virtual Edges getEdges() = 0;
};