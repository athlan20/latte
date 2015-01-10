#include "utils/XUtilsFile.h"
#include "UtilsFileTest.hpp"
//#include "DownloaderTest.hpp"
#include "UpdaterTest.hpp"

#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <iostream>

#pragma comment(lib, "winceUnitTestLib.lib")
//pragma_comment(lib, "##/##/mine.lib")

void TestInit();

int _tmain(int argc, _TCHAR* argv[])
{
	TestInit();
	TestResult tr;
	TestRegistry::runAllTests(tr);
	int a=0;

	std::cin>>a;
	return 0;
}



void TestInit()
{
	XUtilsFile::init();
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