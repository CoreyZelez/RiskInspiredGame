#include "UnitTestRunner.h"
#include "EstateUnitTest.h"
#include "GridUnitTest.h"
#include "CompositeGridUnitTest.h"

void UnitTestRunner::runTests()
{
	GridUnitTest gridUnitTest;
	EstateUnitTest estateUnitTest;
	CompositeGridUnitTest compositeGridUnitTest;

	gridUnitTest.test();
	estateUnitTest.test();
	compositeGridUnitTest.test();
}
