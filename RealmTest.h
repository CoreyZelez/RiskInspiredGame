#pragma once

#include "IUnitTest.h"
class RealmTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void testEstateRemoval();

};

