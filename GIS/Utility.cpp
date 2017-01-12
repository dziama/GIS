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

void verifyNodePointer(const NodePtr& listNode)
{
	if (listNode)
	{
		auto next = listNode->getNext().lock();
		auto prev = listNode->getPrev().lock();

		if (next == nullptr)
		{
			throw runtime_error{ "Given listNode pointer has invalid next node pointer: nullptr!" };
		}

		if (prev == nullptr)
		{
			throw runtime_error{ "Given listNode pointer has invalid previous node pointer: nullptr!" };
		}

		if (next == listNode)
		{
			if (prev != listNode)
			{
				stringstream errstr;
				errstr << "Invalid listNode pointer linking! NextPtr points to the same node while PrevPtr points somwhere else! NodePriority:";
				errstr << listNode->getPriority() << " Node number: " << listNode->getNodeNumber();
				throw runtime_error{ errstr.str().c_str() };
			}
		}

		if (prev == listNode)
		{
			if (next != listNode)
			{
				stringstream errstr;
				errstr << "Invalid listNode pointer linking! NextPtr points to the same node while PrevPtr points somwhere else! NodePriority:";
				errstr << listNode->getPriority() << " Node number: " << listNode->getNodeNumber();
				throw runtime_error{ errstr.str().c_str() };
			}
		}
	}
	else
	{
		throw runtime_error{ "Pointer to list node is nullptr!" };
	}
}

void verifyNodeDoubleLinkedList(const NodePtr& listNode)
{
	verifyNodePointer(listNode);

	auto begin = listNode;
	auto current = begin;
	auto previous = begin;

	unsigned cnt = 0;
	//Moving forward(to right links)
	do
	{
		verifyNodePointer(current);

		previous = current;
		current = current->getNext().lock();
		cnt++;

	} while (current != begin);

	current = begin;
	previous = begin;

	cnt = 0;
	//Moving backwards(left links)
	do
	{
		verifyNodePointer(current);

		previous = current;
		current = current->getPrev().lock();
		cnt++;

	} while (current != begin);
}