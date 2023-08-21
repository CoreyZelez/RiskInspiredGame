#include "IUnitTest.h"
#include <iostream>

void IUnitTest::testReport(std::string testName, bool result, std::string message)
{
	std::string resultMessage = "pass";
	if(!result)
	{
		resultMessage = "fail";
	}

	std::cout << "test: " << testName << std::endl;
	std::cout << "result: "<< resultMessage << std::endl;
	if(message != "")
	{
		std::cout << "message: " << message << std::endl;
	}
	std::cout << std::endl;
}
