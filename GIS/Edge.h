#pragma once

#include "Interfaces.h"

//Klasa reprezentujaca krawedz grafu
class Edge : public IEdge
{
public:
	//Konstruktor
	Edge(EdgeId id, EdgeWeight weight);
	
	//Metody interfejsowe
	virtual EdgeId getId() override;
	virtual EdgeWeight getWeight() override;

	virtual void setFirstVertex(VertexPtr ptr) override;
	virtual void setSecondVertex(VertexPtr ptr) override;
	virtual VertexPtr getFirstVertex() override;
	virtual VertexPtr getSecondVertex() override;
private:

	//Numer Id
	EdgeId m_Id;

	//Waga krawedzi
	EdgeWeight m_Weight;

	//Wezly do ktorych dana krawedz jest podlaczona
	VertexPtr m_FirstVertex;
	VertexPtr m_SecondVertex;
};

