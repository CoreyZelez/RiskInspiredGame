#pragma once
#include "IUnitTest.h"

class EstateTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void testBaronyYield();

};

