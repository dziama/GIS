#pragma once
#include "SFML.h"

class HeapNodeGui
{
public:
	HeapNodeGui(HeapNodePtr ptr, Vector2u tileSize);
	~HeapNodeGui();
private:
	Text m_NodeNumberSprite;
	CircleShape m_NodeSprite;

	Vector2f m_Position;

	HeapNodePtr m_NodePtr;

	static Color m_MinPtr;
	static Color m_Default;
	static Color m_Active;
};

