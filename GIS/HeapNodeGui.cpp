#include "HeapNodeGui.h"

Color HeapNodeGui::m_MinPtr{ 255, 0, 0,0 };
Color HeapNodeGui::m_Default{ 0, 255, 0, 0 };
Color HeapNodeGui::m_Active{ 0, 0, 255, 0 };

HeapNodeGui::HeapNodeGui(Vector2u tileSize, Font& font)
{
	m_NodeSprite = CircleShape{(tileSize.x / 2.0f)};
	m_Font = font;
	m_TileSize = tileSize;
}

HeapNodeGui::~HeapNodeGui()
{
}

void HeapNodeGui::setNode(NodePtr ptr)
{
	m_NodePtr = ptr;
}

void HeapNodeGui::setPosition(Vector2f pos)
{
	m_Position = pos;
}

void HeapNodeGui::draw(RenderWindow& window, FibonacciHeap& heap)
{
	if (m_NodePtr.lock() == heap.peekMinElement().lock())
	{
		m_NodeSprite.setFillColor(m_MinPtr);
	}
	else
	{
		m_NodeSprite.setFillColor(m_Default);
	}

	window.draw(m_NodeSprite);

	auto node = m_NodePtr.lock();

	stringstream ss;
	string priority;
	EdgeWeight weight = node->getPriority();

	if (weight > 100L)
	{
		ss << "INF";
	}
	else
	{
		ss << weight;
	}
	
	ss >> priority;

	m_PriorityText.setFont(m_Font);
	m_PriorityText.setString(priority);
	m_PriorityText.setCharacterSize(m_TileSize.x / 2);
	m_PriorityText.setFillColor(sf::Color::Black);
	m_PriorityText.setPosition(m_Position);

	window.draw(m_PriorityText);
}