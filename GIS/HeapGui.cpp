#include "HeapGui.h"

NodeId HeapGui::m_EmptyTile{-1L};

HeapGui::HeapGui(HeapPtr heap) : m_Heap{heap}
{
	m_Window.create(VideoMode{m_WidthDefault, m_HeightDefault}, "Fibbonacci Heap");
	m_Window.setFramerateLimit(30);
	m_NextFreeLinkId = 1;

	m_TileSize.x = 20;
	m_TileSize.y = 20;
}

HeapGui::~HeapGui()
{
}

void HeapGui::clearMatrix()
{
	for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
	{
		for (unsigned j = 0; j < m_DrawMatrix[i].size(); ++j)
		{
			m_DrawMatrix[i][j] = m_EmptyTile;
		}
	}
}

void HeapGui::writeToMatrix(NodeId val, unsigned x, unsigned y)
{
	if (m_DrawMatrix.size() == 0)
	{
		Row row{};
		row.push_back(m_EmptyTile);
		m_DrawMatrix.push_back(row);
	}

	auto temp = x;
	x = y;
	y = temp;

	if (x > m_DrawMatrix.size() - 1)
	{
		auto diff_x = x - m_DrawMatrix.size() + 1;
		auto row_size = m_DrawMatrix[0].size();

		Row mat_row;
		for (unsigned i = 0; i < row_size; ++i)
		{
			mat_row.push_back(m_EmptyTile);
		}

		for (unsigned i = 0; i < diff_x; ++i)
		{
			m_DrawMatrix.push_back(mat_row);
		}
	}

	if (y > m_DrawMatrix[0].size() - 1)
	{
		auto diff_y = y - m_DrawMatrix[0].size() + 1;
		for (unsigned i = 0; i < m_DrawMatrix.size(); ++i)
		{
			for (unsigned j = 0; j < diff_y; ++j)
			{
				m_DrawMatrix[i].push_back(m_EmptyTile);
			}
		}
	}

	m_DrawMatrix[x][y] = val;
}

void HeapGui::registerDLinkSiblings(NodePtr& first, NodePtr& second)
{
	DLinkSiblings link{};
	if (first->getNext().lock() == second && second->getPrev().lock() == first)
	{
		link.first_sibling = first;
		link.second_sibling = second;
		m_DLinksSiblings.push_back(link);
	}
	else if (first->getPrev().lock() == second && second->getNext().lock() == first)
	{
		link.first_sibling = first;
		link.second_sibling = second;
		m_DLinksSiblings.push_back(link);
	}
	else
	{

	}
}

void HeapGui::registerSLinkParentChild(NodePtr& node)
{
	if (node->getParent().lock() != nullptr)
	{
		SLinkChildParent link{};
		link.child = node;
		link.parent = node->getParent();
		m_SLinksChildPar.push_back(link);
	}
}

void HeapGui::registerDLinkParentChild(NodePtr& node)
{
	if (node->getParent().lock() != nullptr)
	{
		DLinkParentChild link{};
		link.child = node;
		link.parent = node->getParent();
		m_DLinksParChild.push_back(link);
	}
}

long HeapGui::addNodeToDrawMatrix(NodePtr node, DrawMatrix& m, unsigned x, unsigned y)
{
	writeToMatrix(node->getNodeNumber(), x, y);

	if (node->getDegree() == 0)
	{
		return 0L;
	}
	else if (node->getDegree() == 1)
	{
		registerDLinkParentChild(node->getChild().lock());
		addNodeToDrawMatrix(node->getChild().lock(), m, x, y + 2);
		return 0L;
	}
	else if (node->getDegree() == 2)
	{
		addNodeToDrawMatrix(node->getChild().lock()->getPrev().lock(), m, x, y + 2);
		addNodeToDrawMatrix(node->getChild().lock(), m, x + 2, y + 2);
		return 0L;
	}
	else
	{
		y = y + 2;
		long w = 0;

		vector<NodePtr> children;
		auto first_child = node->getChild().lock();
		auto last_child = first_child->getPrev().lock();

		registerDLinkParentChild(first_child);

		auto current = last_child;
		do
		{
			children.push_back(current);
			registerSLinkParentChild(current);
			registerDLinkSiblings(current, current->getPrev().lock());
			current = current->getPrev().lock();
		} while (current != last_child);

		for (auto& child : children)
		{
			w += addNodeToDrawMatrix(child, m, x, y);
			x = xOffset(x, w);
		}

		return w;
	}
}

long HeapGui::xOffset(unsigned x, long w)
{
	return x + 2 + w;
}