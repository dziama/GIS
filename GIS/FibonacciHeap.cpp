#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap() : m_NextFreeNodeNumber{0L}
{
}


FibonacciHeap::~FibonacciHeap()
{
	m_MinElement.lock() = nullptr;
	m_HeapNodes.clear();
}

void FibonacciHeap::insert(VertexPtr ptr, EdgeWeight weight)
{
	//1,2,3,4,5,6,7
	NodePtr new_node{new HeapNode{ptr, weight, m_NextFreeNodeNumber++}};
	m_HeapNodes.insert(HeapPair(ptr.lock()->getId(), new_node));
	new_node = m_HeapNodes[ptr.lock()->getId()];

	new_node->setNext(new_node);
	new_node->setPrev(new_node);

	//9
	insertIntoRootList(new_node);
	
}

bool FibonacciHeap::isEmpty()
{
	return pointerEmpty(m_MinElement);
}

bool FibonacciHeap::pointerEmpty(const HeapNodePtr& ptr)
{
	if (ptr.use_count() == 0L)
	{
		return true;
	}
	else
	{
		return false;
	}
}

NodePtr FibonacciHeap::extractMin()
{
	NodePtr ptr;
	if (pointerEmpty(m_MinElement) == false)
	{
		ptr = removeMinFromRootList();
		m_HeapNodes.erase(ptr->getVertex().lock()->getId());
	}
	return ptr;
}

void FibonacciHeap::removeFromRootList(NodePtr& ptr)
{
	auto next = ptr->getNext().lock();
	auto prev = ptr->getPrev().lock();

	next->setPrev(prev);
	prev->setNext(next);

	ptr->setNext(ptr);
	ptr->setPrev(ptr);
}

void FibonacciHeap::heapLink(NodePtr& toChild, NodePtr& toParent)
{
	removeFromRootList(toChild);

	if (toParent->hasChild())
	{
		auto child = toParent->getChild().lock();
		auto child_prev = child->getPrev().lock();

		toChild->setNext(child);
		child->setPrev(toChild);

		toChild->setPrev(child_prev);
		child_prev->setNext(toChild);
	}
	else
	{
		toChild->setNext(toChild);
		toChild->setPrev(toChild);
	}

	toParent->increaseDegree(1);
	toParent->setChild(toChild);
	toChild->setParent(toParent);
	toChild->unmark();
}

void FibonacciHeap::consolidate()
{
	unsigned aux_table_size = (unsigned)ceil(log(m_HeapNodes.size())) + 1;
	vector<NodePtr> aux_table;
	aux_table.reserve(aux_table_size);

	for (unsigned i = 0; i < aux_table_size; ++i)
	{
		aux_table.push_back(nullptr);
	}

	auto current_node = m_MinElement.lock();
	auto next_node = current_node;
	NodeDegree degree = current_node->getDegree();

	do
	{
		next_node = current_node->getNext().lock();
		removeFromRootList(current_node);

		while (aux_table[degree] != nullptr)
		{
			auto second_node = aux_table[degree];
			if (current_node->getPriority() > second_node->getPriority())
			{
				heapLink(current_node, second_node);
			}
			else
			{
				heapLink(second_node, current_node);
			}
			aux_table[degree] = nullptr;
			++degree;

			if (degree >= aux_table_size)
			{
				break;
			}
		}

		if (degree >= aux_table_size)
		{
			break;
		}
		aux_table[degree] = current_node;
		m_MinElement.lock().reset();

		current_node = next_node;

	} while (current_node->getNext().lock() == current_node);

	for (unsigned itr = 0; itr < aux_table.size(); ++itr)
	{
		if (aux_table[itr] != nullptr)
		{
			auto min_element = m_MinElement.lock();
			insertIntoRootList(aux_table[itr]);
			if (min_element == nullptr || aux_table[itr]->getPriority() < min_element->getPriority())
			{
				m_MinElement = aux_table[itr];
			}
		}
	}
}

void FibonacciHeap::insertIntoRootList(NodePtr& node)
{
	if (m_MinElement.lock() != nullptr)
	{
		auto min_element = m_MinElement.lock();
		auto min_prev = min_element->getPrev().lock();

		min_element->setPrev(node);
		min_prev->setNext(node);

		node->setNext(min_element);
		node->setPrev(min_prev);

		if (node->getPriority() < min_element->getPriority())//8  //set as new minimum element
		{
			m_MinElement = node; //9
		}
	}
	else
	{
		m_MinElement = node;
	}
}

NodePtr FibonacciHeap::removeMinFromRootList()
{
	NodePtr ptr;
	//VertexPtr ptr;
	//get lowest priority element
	auto min_ptr = m_MinElement.lock();
	ptr = min_ptr;

	//get its next and prev elemnet pointers, will be needed
	auto min_next = min_ptr->getNext().lock();
	auto min_prev = min_ptr->getPrev().lock();

	if (min_ptr->hasChild())
	{
		NodeDegree degree = min_ptr->getDegree();
		NodePtr first_child = min_ptr->getChild().lock();
		NodePtr last_child = first_child->getPrev().lock();

		first_child->setParent(HeapNodePtr{});

		if (first_child == last_child)
		{
			min_prev->setNext(first_child);
			first_child->setPrev(min_prev);

			min_next->setPrev(first_child);
			first_child->setNext(min_next);
		}
		else
		{
			auto current = first_child;
			while (current != last_child)
			{
				current->setParent(HeapNodePtr{});
			}

			min_prev->setNext(first_child);
			first_child->setPrev(min_prev);

			min_next->setPrev(last_child);
			last_child->setNext(min_next);
		}
	}
	else
	{
		min_prev->setNext(min_next);
		min_next->setPrev(min_prev);
	}

	if (min_ptr->getNext().lock() == min_ptr)
	{
		m_MinElement = HeapNodePtr{};
	}
	else
	{
		m_MinElement = min_ptr->getNext();
		consolidate();
	}

	return ptr;
}

void FibonacciHeap::printNodeList(ostream& stream, HeapNodePtr& ptr, bool verbose)
{
	NodePtr begin = ptr.lock();
	NodePtr current = begin;

	auto size = getSize();
	if (verbose)
	{
		std::cout << "-----------------------FORWARD----------------------------" << endl;
		do
		{
			stream << " Node: " << current->getNodeNumber()
			<< " Priority: " << current->getPriority()
			<< " Degree: " << current->getDegree()
			<< " Children: " << current->hasChild() << endl;
			current = current->getNext().lock();
		} while (current != begin);

		current = begin;

		std::cout << "-----------------------REVERSE----------------------------" << endl;
		do
		{
			stream << " Node: " << current->getNodeNumber()
				<< " Priority: " << current->getPriority()
				<< " Degree: " << current->getDegree()
				<< " Children: " << current->hasChild() << endl;
			current = current->getPrev().lock();
		} while (current != begin);
	}
	else
	{
		do
		{
			stream << " Node: " << current->getPriority() << "  ";
			current = current->getNext().lock();
		} while (current != begin);
		stream << endl;
	}
	
}

size_t FibonacciHeap::getSize()
{
	return m_HeapNodes.size();
}

HeapNodePtr FibonacciHeap::peekMinElement()
{
	return m_MinElement;
}