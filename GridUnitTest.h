#pragma once
#include "IUnitTest.h"

class GridUnitTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	// Test extraction of a square.
	void extractInteriorPolygonTest1() const;

	// Test extraction polygon shaped like a T tetris block. Interior is 1 wide.
	void extractInteriorPolygonTest2() const;

	// Extract two simple polygons. 
	void extractInteriorPolygonsTest1() const;

	// Constructs a grid shaped as multiple disjoint polygons.
	void constructorTest1() const;
};

