#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap() : m_NextFreeNodeNumber{0L}
{
}


FibonacciHeap::~FibonacciHeap()
{
}

void FibonacciHeap::insert(VertexPtr ptr, EdgeWeight weight)
{
	//1,2,3,4,5,6,7
	NodePtr new_node{new HeapNode{ptr, weight, m_NextFreeNodeNumber++}}; 
	new_node->setNext(new_node);
	new_node->setPrev(new_node);

	//9
	insertIntoRootList(new_node);
	m_HeapNodes.insert(HeapPair(ptr.lock()->getId(), new_node));
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

VertexPtr FibonacciHeap::extractMin()
{
	VertexPtr ptr;
	if (pointerEmpty(m_MinElement) == false)
	{
		ptr = removeMinFromRootList();
		m_HeapNodes.erase(ptr.lock()->getId());
	}
	return ptr;
}

void FibonacciHeap::heapLink()
{
	//unique_ptr<HeapNodePtr> support_array{new HeapNodePtr[m_NodesCounter]};

}

void FibonacciHeap::consolidate()
{

}

void FibonacciHeap::insertIntoRootList(NodePtr node)
{
	if (isEmpty() == false)
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

VertexPtr FibonacciHeap::removeMinFromRootList()
{
	VertexPtr ptr;
	//get lowest priority element
	auto min_ptr = m_MinElement.lock();
	ptr = min_ptr->getVertex();

	//get its next and prev elemnet pointers, will be needed
	auto min_next = min_ptr->getNext().lock();
	auto min_prev = min_ptr->getPrev().lock();

	if (min_ptr->hasChild())
	{
		NodeDegree degree = min_ptr->getDegree();
		NodePtr first_child = min_ptr->getChild().lock();
		NodePtr last_child = first_child;

		first_child->setParent(HeapNodePtr{});

		while (pointerEmpty(last_child->getNext()) == false)
		{
			last_child = last_child->getNext().lock();
			last_child->setParent(HeapNodePtr{});
		}
		
		if (first_child == last_child)
		{
			min_prev->setNext(first_child);
			first_child->setPrev(min_prev);

			min_next->setPrev(first_child);
			first_child->setNext(min_next);
		}
		else
		{
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

void FibonacciHeap::printRootList(ostream& stream)
{
	NodePtr current = m_MinElement.lock();
	auto size = getSize();

	std::cout << "-----------------------FORWARD----------------------------" << endl;
	for (unsigned cnt = 0; cnt < size; ++cnt)
	{
		stream << " Node: " << current->getNodeNumber()
			<< " Priority: " << current->getPriority()
			<< " Degree: " << current->getDegree()
			<< " Children: " << current->hasChild() << endl;
		current = current->getNext().lock();
	}

	current = m_MinElement.lock();

	std::cout << "-----------------------REVERSE----------------------------" << endl;
	for (unsigned cnt = 0; cnt < size; ++cnt)
	{
		stream << " Node: " << current->getNodeNumber()
			<< " Priority: " << current->getPriority()
			<< " Degree: " << current->getDegree()
			<< " Children: " << current->hasChild() << endl;
		current = current->getPrev().lock();
	}
}

size_t FibonacciHeap::getSize()
{
	return m_HeapNodes.size();
}