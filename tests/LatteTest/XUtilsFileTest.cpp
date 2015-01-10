#include "stdafx.h"
#include "lib/utils/XUtilsFile.h"

#include <windows.h>

#include <stdio.h>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LatteTest
{
	const std::string fileExistFileName = "./testFileExit";
	const std::string getFileDataStr = "test File exit.\n";
	const std::string deletFileName = "testDeleteFile";
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(init)
		{
			XUtilsFile::init();

		}

		TEST_METHOD_INITIALIZE(initMethod)
		{
			std::ofstream of;
			of.open(fileExistFileName.c_str());
			of << getFileDataStr;
			of.close();
		}

		TEST_METHOD(isFileExist)
		{
			
			// TODO:  在此输入测试代码
			//std::string fileName = "D:\\my_work\\cpp\\test\\Updater\\unitTests\\DownloaderTest\\bin\\Debug\\testFileExit2";



			std::ifstream f(fileExistFileName.c_str());
			bool acturalExist = f.good();

			bool isExist = XUtilsFile::isFileExist(fileExistFileName);
			Assert::AreEqual(acturalExist, isExist);
		}

		TEST_METHOD(getFileData)
		{
			size_t fileSize = 32;
			std::string toCompareStr = XUtilsFile::getFileData(fileExistFileName);
			//int res = getFileDataStr.compare(toCompareStr);
			//Assert::AreEqual(0, res);
		}

		TEST_METHOD(deleteFile)
		{
			//先创建文件
			std::ofstream of;
			of.open(deletFileName.c_str());
			of << getFileDataStr;
			of.close();

			XUtilsFile::deleteFile(deletFileName);
			bool isExist = XUtilsFile::isFileExist(deletFileName);
			Assert::AreEqual(false,isExist);
		}

		TEST_METHOD(moveFiles)
		{
			XUtilsFile::copyFile("a\\*", "b\\");
		}

		TEST_METHOD_CLEANUP(cleanupMethod)
		{
			XUtilsFile::deleteFile(fileExistFileName);
		}

		TEST_CLASS_CLEANUP(clean)
		{
			
		}

	};
}