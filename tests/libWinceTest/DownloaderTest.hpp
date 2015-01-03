#ifndef _DOWNLOADERTEST_H__
#define _DOWNLOADERTEST_H__

#include "base/global.h"
#include "testGlobal.hpp"
#include "utils/XUtilsFile.h"
#include "net/XDownloader.h"
#include "core/XApplication.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <string>
#include <fstream>
#include <iostream>



const std::string localNetworkAddr = "http://116.236.150.110/test/resource.json";//"http://www.latte.com/php/test/github_xpgod.rar";// "http://116.236.150.110/test/web.zip";
const std::string localNetworkErrAddr = "http://www.latte.com/php/test/resource.json";
const std::string resServerRoot = "http://www.latte.com/php/test/";
const std::string storagePathAddr = "resource.json";

void downloadSyncSuccess(const std::string & url, const std::string & localPathName, const std::string & customId)
{
	XLOG("success download");
}

TEST(XDownloader,testEnd)
{
	XLOG("downloader end");
}

TEST(XDownloader,downloadSync)
{
	boost::shared_ptr<XDownloader> downloader = boost::shared_ptr<XDownloader>(new XDownloader());
	downloader->setSuccessCallback(downloadSyncSuccess);
	downloader->downloadSync(localNetworkAddr, storagePathAddr.c_str(), "00001");
	bool isExist = XUtilsFile::isFileExist(storagePathAddr);
	downloader.reset();
}

TEST(XDownloader,testInit)
{
	XApplication::getInstance()->setAnimationInterval(1.0/20);
	XApplication::getInstance()->runInBack();
}


#endif