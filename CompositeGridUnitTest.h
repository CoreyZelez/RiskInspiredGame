#pragma once
#include "IUnitTest.h"

class CompositeGridUnitTest : public IUnitTest
{
public:
	virtual void test() override;

private:
	void testSetColor() const;

};

