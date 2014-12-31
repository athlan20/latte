#include "UtilsFileTest.hpp"

#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <iostream>

#pragma comment(lib, "winceUnitTestLib.lib")
//pragma_comment(lib, "##/##/mine.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	TestResult tr;
	TestRegistry::runAllTests(tr);
	int a=0;

	std::cin>>a;
	return 0;
}

SimpleString StringFrom(const std::string& value)
{
	return SimpleString(value.c_str());
}

//TEST( Stack, test_simple )
//{
//	//ClassA a;
//
//	//LONGS_EQUAL(66, a.Method1());
//	//DOUBLES_EQUAL(1.9, a.Method2(), 0.1);
//	//CHECK_EQUAL("string", a.Method3());
//	//CHECK(!a.Method4());
//	
//	{
//		UtilsFileTest test;
//		test.testFileExist();
//	}
//	
//
//}