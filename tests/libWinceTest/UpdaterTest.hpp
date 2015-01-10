#ifndef _UPDATERTEST_H_
#define _UPDATERTEST_H_

#include "testGlobal.hpp"
#include "utils/XUtilsFile.h"
#include "base/global.h"
#include "net/updater/XUpdater.h"
#include "CppUnitLite\TestHarness.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <fstream>
#include <iostream>

boost::shared_ptr<XUpdater> updater;

TEST(XUpdater,renameFile)
{

}

TEST(XUpdater,initTest)
{
	updater = boost::shared_ptr<XUpdater>(new XUpdater("http://116.236.150.110/test/package/","http://116.236.150.110/test/package/resource.json",XUtilsFile::getAbsolutePath("tmpPackage\\").c_str()));
	updater->upgrade(XUtilsFile::getRelativePath("package\\"),XUtilsFile::getRelativePath("package\\resource.json"));
}

#endif