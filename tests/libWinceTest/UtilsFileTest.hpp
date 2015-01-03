#ifndef _UTILSFILETEST_H__
#define _UTILSFILETEST_H__

#include "testGlobal.hpp"
#include "utils/XUtilsFile.h"
#include "base/global.h"
#include "CppUnitLite\TestHarness.h"

#include <string>
#include <fstream>
#include <iostream>

const std::string fileExistFileName = "testFileExit";
const std::string getFileDataStr = "test File exit.\n";
const std::string deletFileName = "testDeleteFile";



TEST (XUtilsFile,fileExist)
{
	std::ofstream of;
	std::string basePath = XUtilsFile::getWorkPath();
	of.open((basePath+fileExistFileName).c_str());
	of << getFileDataStr;
	of.close();

	std::ifstream f((basePath+fileExistFileName).c_str());
	bool acturalExist = f.good();
	f.close();

	bool isExist = XUtilsFile::isFileExist(fileExistFileName);
	XUtilsFile::deleteFile(fileExistFileName);
	CHECK_EQUAL(acturalExist,isExist);
}

TEST(XUtilsFile,getFileData)
{
	std::ofstream of;
	std::string basePath = XUtilsFile::getWorkPath();
	of.open((basePath+fileExistFileName).c_str());
	of << getFileDataStr;
	of.close();

	size_t fileSize = 32;
	std::string toCompareStr = XUtilsFile::getFileData(fileExistFileName);
	int res = getFileDataStr.compare(toCompareStr);
	XUtilsFile::deleteFile(fileExistFileName);

	LONGS_EQUAL(0, res);
}

TEST(XUtilsFile,writeFileData)
{
	std::string distFile = "writeFile.txt";
	XUtilsFile::writeFileData(distFile,"writeFile");
	std::string toCompareStr = XUtilsFile::getFileData(distFile);

	XUtilsFile::deleteFile(distFile);
	CHECK_EQUAL(toCompareStr,"writeFile");
}

TEST(XUtilsFile,deleteFile)
{
	std::string deletFileName = "testDeleteFile";
	std::string basePath = XUtilsFile::getWorkPath();
	//先创建文件
	std::ofstream of;
	of.open((basePath+deletFileName).c_str());
	of << getFileDataStr;
	of.close();
	//bool isExist = XUtilsFile::isFileExist(deletFileName);
	//CHECK_EQUAL(true, isExist);

	XUtilsFile::deleteFile(deletFileName);
	bool isExist = XUtilsFile::isFileExist(deletFileName);
	CHECK_EQUAL(false, isExist);
}

TEST(XUtilsFile,renameFile)
{
	std::string oldFileName = "oldFileName";
	std::string distFileName = "newFileName";
	XUtilsFile::writeFileData(oldFileName,"writeFile");
	XUtilsFile::renameFile(oldFileName,distFileName);
	
	bool isExist = XUtilsFile::isFileExist(distFileName);
	bool oldFileExist = XUtilsFile::isFileExist(oldFileName);
	XUtilsFile::deleteFile(distFileName);
	CHECK( (oldFileExist==false) && (isExist==true) );
}

TEST(XUtilsFile,getFilesInDir)
{
	std::vector<std::string> allFilePath = XUtilsFile::getFilesInDir();
	std::vector<std::string>::iterator it = allFilePath.begin();
	for(;it!=allFilePath.end();++it)
	{
		//XLOG(it->c_str());
	}

	CHECK( (allFilePath.size()>2) );
}

#endif