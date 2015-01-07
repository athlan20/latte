#include "stdafx.h"
#include "lib/utils/XUtilsFormatter.h"

#include <windows.h>

#include <stdio.h>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LatteTest
{

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(init)
		{

		}

		TEST_METHOD_INITIALIZE(initMethod)
		{

		}

		TEST_METHOD(utilsFormatter_addslashes)
		{
			std::string res = XUtilsFormatter::addslashes("\a\\b\\\c\'ac\'bbbb\"dd\"");

			Assert::IsTrue(res.compare("\a\\\\b\\\\c\\\'ac\\\'bbbb\\\"dd\\\"")==0);
		}
	
		TEST_METHOD_CLEANUP(cleanupMethod)
		{

		}

		TEST_CLASS_CLEANUP(clean)
		{
			
		}

	};
}