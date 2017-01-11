#pragma once
#include "GIS\Graph.h"
#include "GIS\FibonacciHeap.h"
#include "GIS\Utility.h"

class FibbonacciHeapTestClass
{
public:
	FibbonacciHeapTestClass();
	~FibbonacciHeapTestClass();

	void testHeapLink();
	void testExtractMin();
	void testMoveNodeChildren();
	void testCut();
	void testDecreaseKey();
};

