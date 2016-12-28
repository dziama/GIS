#pragma once

#include "Interfaces.h"

class Edge : public IEdge
{
public:
	Edge(EdgeId id, EdgeWeight weight);
	
	virtual EdgeId getId() override;
	virtual EdgeWeight getWeight() override;

	virtual void setFirstVertex(VertexPtr ptr) override;
	virtual void setSecondVertex(VertexPtr ptr) override;
	virtual VertexPtr getFirstVertex() override;
	virtual VertexPtr getSecondVertex() override;
private:

	EdgeId m_Id;
	EdgeWeight m_Weight;
	VertexPtr m_FirstVertex;
	VertexPtr m_SecondVertex;
};

