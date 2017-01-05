#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap() : m_NextFreeNodeNumber{1L}
{
}


FibonacciHeap::~FibonacciHeap()
{
	m_MinElement.lock() = nullptr;
	m_HeapNodes.clear();
}

HeapNodeId FibonacciHeap::insert(VertexPtr ptr, EdgePtr edge)
{
	HeapNodeId id = m_NextFreeNodeNumber++;
	//1,2,3,4,5,6,7
	NodePtr new_node{new HeapNode{ptr, edge, id}};
	m_HeapNodes.insert(HeapPair(new_node->getNodeNumber(), new_node));
	new_node = m_HeapNodes[new_node->getNodeNumber()];

	new_node->setNext(new_node);
	new_node->setPrev(new_node);

	//9
	insertIntoRootList(new_node);
	
	return id;
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
	if (m_MinElement.lock() != nullptr)
	{
		ptr = removeMinFromRootList();
		m_HeapNodes.erase(ptr->getNodeNumber());
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

void FibonacciHeap::consolidate(NodePtr begin)
{
	vector<NodePtr> aux_table;
	vector<NodePtr> nodes_to_visit;
	
	auto current_node = begin;
	do
	{
		nodes_to_visit.push_back(current_node);
		current_node = current_node->getNext().lock();
	} while (current_node != begin);

	NodeDegree degree = 0;

	auto& auxTableSentry = [&aux_table](NodeDegree d) -> bool
	{
		if (aux_table.size() == 0)
		{
			aux_table.push_back(nullptr);
		}

		if(aux_table.size() - 1 < d)
		{
			auto diff = d - aux_table.size() + 1;
			for (unsigned i = 0; i < diff; ++i)
			{
				aux_table.push_back(nullptr);
			}
		}

		if (aux_table[d] == nullptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	for (auto& visted_node : nodes_to_visit)
	{
		degree = visted_node->getDegree();
		while (auxTableSentry(degree))
		{
			auto y = aux_table[degree];
			if (visted_node->getPriority() > y->getPriority())
			{
				visted_node.swap(y);
			}

			heapLink(y, visted_node);
			
			aux_table[degree] = nullptr;
			++degree;
			auxTableSentry(degree);
		}

		auxTableSentry(degree);
		aux_table[degree] = visted_node;
	}

	m_MinElement = HeapNodePtr{};

	for (auto& node : aux_table)
	{
		if (node != nullptr)
		{
			insertIntoRootList(node);
			if (node->getPriority() < m_MinElement.lock()->getPriority())
			{
				m_MinElement = node;
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

NodePtr FibonacciHeap::moveNodeChildrenToRootList(NodePtr& node)
{
	auto next = node->getNext().lock();
	auto prev = node->getPrev().lock();

	if (node->hasParent())
	{
		throw exception{ "Node to be extracted cannot have parent!" };
	}

	if (node->hasChild())
	{
		if (node->getDegree() == 1)
		{
			NodePtr child = node->getChild().lock();

			child->setParent(HeapNodePtr{});
			child->setNext(next);
			child->setPrev(prev);

			next->setPrev(child);
			prev->setNext(child);
		}
		else
		{
			NodePtr first_child = node->getChild().lock();
			NodePtr last_child = first_child->getPrev().lock();

			prev->setNext(first_child);
			first_child->setPrev(prev);

			next->setPrev(last_child);
			last_child->setNext(next);

			auto current_child = first_child;
			do
			{
				current_child->setParent(HeapNodePtr{});
				current_child = current_child->getNext().lock();
			} while (current_child != last_child);
		}
	}
	else
	{
		next->setPrev(prev);
		prev->setNext(next);
	}

	node->setChild(HeapNodePtr{});

	m_MinElement = next;

	return next;
}

NodePtr FibonacciHeap::removeMinFromRootList()
{
	auto min_ptr = m_MinElement.lock();

	removeFromRootList(min_ptr);

	auto node = moveNodeChildrenToRootList(min_ptr);

	verifyNodeDoubleLinkedList(node);

	consolidate(node);

	verifyNodeDoubleLinkedList(node);

	return min_ptr;
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