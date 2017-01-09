#include "FibonacciHeap.h"

FibonacciHeap::FibonacciHeap() : m_NextFreeNodeNumber{0L}
{
}


FibonacciHeap::~FibonacciHeap()
{
	m_MinElement.lock() = nullptr;
	m_HeapNodes.clear();
}

void FibonacciHeap::cut(NodePtr child, NodePtr parent)
{
	auto child_next = child->getNext().lock();
	auto child_prev = child->getPrev().lock();

	if (parent->getChild().lock() == child)
	{
		if (parent->getDegree() == 1)
		{
			parent->setChild(HeapNodePtr{});
		}
		else if (child_next->getDegree() == child->getDegree() - 1)
		{
			parent->setChild(child_next);
		}
		else if(child_prev->getDegree() == child->getDegree() - 1)
		{
			parent->setChild(child_prev);
		}
		else
		{
			parent->setChild(child_prev);
			throw exception{ "Cut: Error setting new child node of parent when cutting one of its children!" };
		}
	}

	removeFromList(child);
	child->setParent(HeapNodePtr{});
	parent->decreaseDegree();
	child->unmark();

	insertIntoRootList(child);
}

void FibonacciHeap::cascadeCut(NodePtr node)
{
	auto node_parent = node->getParent().lock();
	if (node_parent != nullptr)
	{
		if (node_parent->isMarked() == false)
		{
			node_parent->mark();
		}
		else
		{
			cut(node, node_parent);
			cascadeCut(node_parent);
		}
	}
}

NodePtr FibonacciHeap::find(VertexPtr ptr)
{
	auto id = ptr.lock()->getId();
	NodePtr node_ptr{};

	for (auto& heapNode : m_HeapNodes)
	{
		auto vertex = heapNode.second->getVertex().lock();
		if (vertex->getId() == id)
		{
			node_ptr = heapNode.second;
		}
	}

	if (node_ptr == nullptr)
	{
		throw exception{ "Node not found in heap!" };
	}
	return node_ptr;
}

void FibonacciHeap::decreaseKey(VertexPtr ptr, EdgeWeight weight)
{
	auto node = find(ptr);
	auto node_parent = node->getParent().lock();

	if (node->getPriority() < weight)
	{
		throw exception{ "Edge to be changed has higher weight than previous!" };
	}

	if ((node_parent != nullptr) && (node->getPriority() < node_parent->getPriority()))
	{
		cut(node, node_parent);
		cascadeCut(node_parent);
	}

	if (node->getPriority() < m_MinElement.lock()->getPriority())
	{
		m_MinElement = node;
	}
}

HeapNodeId FibonacciHeap::insert(VertexPtr ptr, EdgeWeight weight)
{
	if (ptr.lock() == nullptr)
	{
		throw exception{ "FibbonacciHeap: VertexPtr passed as nullptr!" };
	}

	HeapNodeId id = m_NextFreeNodeNumber++;
	NodePtr new_node{};
	if (weight == 0)
	{
		new_node = NodePtr(new HeapNode{ ptr, id });
	}
	else
	{
		new_node = NodePtr(new HeapNode{ ptr, id, weight});
	}
		
	//1,2,3,4,5,6,7
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

void FibonacciHeap::removeFromList(NodePtr& ptr)
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
	removeFromList(toChild);

	if (toParent->getDegree() > 1)
	{
		auto child = toParent->getChild().lock();
		auto child_next = child->getNext().lock();

		toChild->setPrev(child);
		child->setNext(toChild); 

		toChild->setNext(child_next);
		child_next->setPrev(toChild);
	}
	else if (toParent->getDegree() == 1)
	{
		auto child = toParent->getChild().lock();

		toChild->setPrev(child);
		child->setNext(toChild);

		toChild->setNext(child);
		child->setPrev(toChild);
	}
	else
	{
		toChild->setNext(toChild);
		toChild->setPrev(toChild);
	}

	toParent->increaseDegree();
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

		if (degree == 0)
		{
			removeFromList(visted_node);
		}

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
			NodePtr last_child = first_child->getNext().lock();

			prev->setNext(last_child);
			last_child->setPrev(prev);

			first_child->setNext(next);
			next->setPrev(first_child);

			auto current_child = first_child;
			do
			{
				current_child->setParent(HeapNodePtr{});
				current_child = current_child->getPrev().lock();
			} while (current_child != first_child);
		}
	}
	else
	{
		next->setPrev(prev);
		prev->setNext(next);
	}

	node->setChild(HeapNodePtr{});

	m_MinElement = prev;

	return next;
}

NodePtr FibonacciHeap::removeMinFromRootList()
{
	auto min_ptr = m_MinElement.lock();

	auto node = moveNodeChildrenToRootList(min_ptr);

	consolidate(node);

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

void FibonacciHeap::printNode(ostream& stream, HeapNodePtr& ptr)
{
	auto node = ptr.lock();

	if (node == nullptr)
	{
		return;
	}

	if (node->getDegree() == 0)
	{
		stream << node->getPriority();
	}
	else if (node->getDegree() == 1)
	{
		stream << node->getPriority() << "|";
	}
	else
	{
		auto child = node->getChild().lock();

	}
}