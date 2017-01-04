#pragma once
#include "Interfaces.h"

struct DLinkParentChild
{
	HeapNodePtr parent;
	HeapNodePtr child;
	LinkId id;
};

struct SLinkChildParent
{
	HeapNodePtr parent;
	HeapNodePtr child;
	LinkId id;
};

struct DLinkSiblings
{
	HeapNodePtr first_sibling;
	HeapNodePtr second_sibling;
	LinkId id;
};

typedef vector<DLinkParentChild> DLinksParChild;
typedef vector<SLinkChildParent> SLinksChildPar;
typedef vector<DLinkSiblings> DLinksSiblings;