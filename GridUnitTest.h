#pragma once
#include "IUnitTest.h"

class GridUnitTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void extractInteriorPolygonTest() const;
};

