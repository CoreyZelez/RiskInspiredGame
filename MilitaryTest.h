#pragma once
#include "IUnitTest.h"

class MilitaryTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void test1();
};

