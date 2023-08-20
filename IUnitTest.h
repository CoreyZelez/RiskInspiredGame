#pragma once
#include <string>

class IUnitTest
{
public:
	virtual void test() = 0;

protected:
	void testReport(std::string testName, bool result, std::string message = "");
};

