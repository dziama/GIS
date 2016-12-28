#pragma once

#include "Interfaces.h"

class Vertex : public IVertex
{
public:
	Vertex(VertexId id);

	virtual VertexId getId() override;
	virtual void addEdge(EdgePtr ptr) override;

private:

	VertexId m_Id;
	VertexId m_ParentVertexId;
	VertexPriority m_Priority;
	vector<EdgeId> m_Edges;
};

