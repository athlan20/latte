#ifndef _UTILSFILETEST_H__
#define _UTILSFILETEST_H__

#include "utils/XUtilsFile.h"

#include "CppUnitLite\TestHarness.h"

#include <string>
#include <fstream>
#include <iostream>

const std::string fileExistFileName = "testFileExit";
const std::string getFileDataStr = "test File exit.\n";
const std::string deletFileName = "testDeleteFile";



TEST (XUtilsFile,fileExist)
{
	XUtilsFile::init();
	std::ofstream of;
	std::string basePath = XUtilsFile::getWorkPath();
	of.open((basePath+fileExistFileName).c_str());
	of << getFileDataStr;
	of.close();

	std::ifstream f((basePath+fileExistFileName).c_str());
	bool acturalExist = f.good();

	bool isExist = XUtilsFile::isFileExist(fileExistFileName);
	CHECK_EQUAL(acturalExist,isExist);
}

#endif