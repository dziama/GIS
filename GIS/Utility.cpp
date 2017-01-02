#include "Utility.h"

void readMatrix(ifstream& file, Matrix& mat)
{
	string line;
	stringstream sstream;
	EdgeWeight weight;

	int row = 0;
	while (getline(file, line))
	{
		sstream << line;
		mat.push_back(vector<EdgeWeight>());
		weight = 0L;
		do
		{
			sstream >> weight;
			mat[row].push_back(weight);
		} while (sstream.eof() == false);
		++row;
		sstream = stringstream{};
	}

	for (auto& row : mat)
	{
		if (row.size() != mat.size())
		{
			break;
			//throw exe, matrix size incorrect
		}
	}
}

void verifyNodePointer(NodePtr& listNode)
{
	if (listNode)
	{
		auto next = listNode->getNext().lock();
		auto prev = listNode->getPrev().lock();

		if (next == nullptr)
		{
			throw exception{ "Given listNode pointer has invalid next node pointer: nullptr!" };
		}

		if (prev == nullptr)
		{
			throw exception{ "Given listNode pointer has invalid previous node pointer: nullptr!" };
		}

		if (next == listNode)
		{
			if (prev != listNode)
			{
				throw exception{ "Invalid listNode pointer linking! NextPtr points to the same node while PrevPtr points somwhere else!" };
			}
		}

		if (prev == listNode)
		{
			if (next != listNode)
			{
				throw exception{ "Invalid listNode pointer linking! NextPtr points to the same node while PrevPtr points somwhere else!" };
			}
		}
	}
	else
	{
		throw exception{ "Pointer to list node is nullptr!" };
	}
}

void verifyNodeDoubleLinkedList(NodePtr& listNode)
{
	verifyNodePointer(listNode);

	auto begin = listNode;
	auto current = begin;
	auto previous = begin;

	//Moving forward(to right links)
	do
	{
		verifyNodePointer(current);

		previous = current;
		current = current->getNext().lock();

	} while (current != begin);

	current = begin;
	previous = begin;

	//Moving backwards(left links)
	do
	{
		verifyNodePointer(current);

		previous = current;
		current = current->getPrev().lock();

	} while (current != begin);
}