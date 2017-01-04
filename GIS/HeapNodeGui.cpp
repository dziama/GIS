#include "HeapNodeGui.h"

Color HeapNodeGui::m_MinPtr{ 255, 0, 0,0 };
Color HeapNodeGui::m_Default{ 0, 255, 0, 0 };
Color HeapNodeGui::m_Active{ 0, 0, 255, 0 };

HeapNodeGui::HeapNodeGui(HeapNodePtr ptr, Vector2u tileSize) : m_NodePtr{ptr}
{
	m_NodeSprite = CircleShape{(tileSize.x / 2.0f)};
	m_NodeSprite.setFillColor(m_Default);
}


HeapNodeGui::~HeapNodeGui()
{
}
