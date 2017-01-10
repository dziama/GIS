#pragma once
#include "SFML.h"
#include "FibonacciHeap.h"

class HeapNodeGui
{
public:
	HeapNodeGui(Vector2u tileSize, Font& font);
	~HeapNodeGui();

	void setNode(NodePtr ptr);
	void draw(RenderWindow& window, FibonacciHeap& heap);
	void setPosition(Vector2f pos);

private:
	Text m_NodeNumberSprite;
	CircleShape m_NodeSprite;

	Vector2f m_Position;

	Vector2u m_TileSize;

	Font m_Font;
	Text m_PriorityText;

	HeapNodePtr m_NodePtr;

	static Color m_MinPtr;
	static Color m_Default;
	static Color m_Active;
};

