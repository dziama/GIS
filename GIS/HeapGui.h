#pragma once
#include "SFML.h"
#include "FibonacciHeap.h"
#include "Links.h"

typedef vector<HeapNodeId> Row;
typedef vector<Row> DrawMatrix;

class HeapGui
{
public:
	HeapGui(HeapPtr heap);
	~HeapGui();

	void clearMatrix();
	void drawHeap(IGraph& graph);
private:
	Window m_Window;
	View m_ActiveView;

	HeapPtr m_Heap;

	static HeapNodeId m_EmptyTile;

	unsigned m_WidthDefault = 800;
	unsigned m_HeightDefault = 600;

	LinkId m_NextFreeLinkId;

	Vector2u m_TileSize;

	DrawMatrix m_DrawMatrix;

	DLinksParChild m_DLinksParChild;
	SLinksChildPar m_SLinksChildPar;
	DLinksSiblings m_DLinksSiblings;

	void writeToMatrix(HeapNodeId val, unsigned x, unsigned y);
	void registerDLinkSiblings(NodePtr& first, NodePtr& second);
	void registerSLinkParentChild(NodePtr& node);
	void registerDLinkParentChild(NodePtr& node);
	long addNodeToDrawMatrix(NodePtr node, DrawMatrix& m, unsigned x, unsigned y);
	long xOffset(unsigned x);
	
};

