#pragma once
#include "IUnitTest.h"

class EstateUnitTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void testBaronyOwnership();
	void testBaronyYield();

};

