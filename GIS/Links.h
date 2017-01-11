#pragma once
#include "Interfaces.h"

//Struktury reprezetujace powiazania miedzy wezlami, do celow graficznych

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