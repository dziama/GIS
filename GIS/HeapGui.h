#pragma once
#include "SFML.h"
#include "FibonacciHeap.h"
#include "Links.h"
#include "Graph.h"

typedef vector<HeapNodeId> Row;
typedef vector<Row> DrawMatrix;

class HeapGui
{
public:
	HeapGui(FibonacciHeap& heap, Graph& graph);
	~HeapGui();

	void clearMatrix();
	void drawHeap();
	void showGui();

private:
	RenderWindow m_Window;
	View m_ActiveView;

	FibonacciHeap m_Heap;
	Graph m_Graph;

	static HeapNodeId m_EmptyTile;

	unsigned m_WidthDefault = 800;
	unsigned m_HeightDefault = 600;
	unsigned m_ViewWidth = 800;
	unsigned m_ViewHeight = 600;

	LinkId m_NextFreeLinkId;

	Vector2u m_TileSize;

	DrawMatrix m_DrawMatrix;

	Time m_DelayTime;

	DLinksParChild m_DLinksParChild;
	SLinksChildPar m_SLinksChildPar;
	DLinksSiblings m_DLinksSiblings;

	void writeToMatrix(HeapNodeId val, unsigned x, unsigned y);
	void registerDLinkSiblings(NodePtr& first, NodePtr& second);
	void registerSLinkParentChild(NodePtr& node);
	void registerDLinkParentChild(NodePtr& node);
	long addNodeToDrawMatrix(NodePtr node, DrawMatrix& m, unsigned x, unsigned y);
	long xOffset(unsigned x);
	Vector2u getWindowCenter();



	void processEvents();
};

