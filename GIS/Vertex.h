#pragma once

#include "Interfaces.h"

class Vertex : public IVertex
{
public:
	Vertex(VertexId id);

	virtual VertexId getId() override;
	virtual void addEdge(EdgePtr ptr) override;
	virtual vector<EdgeId> getEdges() override;

private:

	VertexId m_Id;
	vector<EdgePtr> m_Edges;
};

