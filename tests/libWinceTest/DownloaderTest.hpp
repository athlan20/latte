#ifndef _DOWNLOADERTEST_H__
#define _DOWNLOADERTEST_H__

#include "base/global.h"
#include "testGlobal.hpp"
#include "utils/XUtilsFile.h"
#include "net/XDownloader.h"
#include "core/XApplication.h"

#include "json/json.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

#include <string>
#include <fstream>
#include <iostream>



const std::string localNetworkAddr = "http://116.236.150.110/test/resource.json";//"http://www.latte.com/php/test/github_xpgod.rar";// "http://116.236.150.110/test/web.zip";
const std::string localNetworkErrAddr = "http://www.latte.com/php/test/resource.json";
const std::string resServerRoot = "http://116.236.150.110/test/";//"http://www.latte.com/php/test/";
const std::string storagePathAddr = "package.json";
bool hasAsysFinish = false;
bool hasAsysProgreassCall = false;

void downloadSyncSuccess(const std::string & url, const std::string & localPathName, const std::string & customId)
{
	XLOG("success download");
}
void downloadError(const XDownloader::Error& e)
{
	XLOG("download error");
}

void downloadASyncSuccess(const std::string & url, const std::string & localPathName, const std::string & customId)
{
	hasAsysFinish=true;
}
void downloadASyncProgress(double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId)
{
	hasAsysProgreassCall=true;
	XLOGP("totalToDownload: %lf",totalToDownload);
}

TEST(XDownloader,testEnd)
{

}

TEST(XDownloader,downloadASync)
{
	
	boost::shared_ptr<XDownloader> downloader = boost::shared_ptr<XDownloader>(new XDownloader());
	downloader->setSuccessCallback(downloadASyncSuccess);
	downloader->setProgressCallback(downloadASyncProgress);
	downloader->downloadAsync(localNetworkAddr, storagePathAddr.c_str(), "00001");
	while (!hasAsysFinish)
	{
		Sleep(0);
	}
	bool isExist = XUtilsFile::isFileExist(storagePathAddr);
	//Sleep(1000);//等待线程先退出完毕
	XUtilsFile::deleteFile("resource.json");
	XUtilsFile::deleteFile("package");
	if (hasAsysFinish && hasAsysProgreassCall)
	{
		//检查是否下载到文件
		CHECK_EQUAL(true,isExist);
	}
	else
	{
		CHECK_EQUAL(true,false);
	}
}

TEST(XDownloader,queueDownloadCall)
{
	boost::shared_ptr<XDownloader> downloader = boost::shared_ptr<XDownloader>(new XDownloader());
	Json::Reader jReader;
	Json::Value root;
	std::string data = XUtilsFile::getFileData("res.json");
	jReader.parse(data,root);
	Json::Value files = root["files"];
	Json::Value::Members member = files.getMemberNames();
	boost::unordered_map<std::string, XDownloader::XDownloadUnit> units;
	for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
	{
		std::string fileKey = *iter;
		std::string fileMD5 = files[fileKey].asString();

		XDownloader::XDownloadUnit unit;
		unit.srcUrl = resServerRoot + fileKey;
		unit.storagePath = fileKey;
		units[fileKey] = unit;
	}
	downloader->queueDownloadSync(units);
}

TEST(XDownloader,downloadSync)
{
	boost::shared_ptr<XDownloader> downloader = boost::shared_ptr<XDownloader>(new XDownloader());
	downloader->setSuccessCallback(downloadSyncSuccess);
	downloader->setErrorCallback(downloadError);
	downloader->downloadSync(localNetworkAddr, "res.json", "00001");
	bool isExist = XUtilsFile::isFileExist("res.json");
	XUtilsFile::deleteFile("resource.json");
	CHECK_EQUAL(true,isExist);
}

TEST(XDownloader,testInit)
{
	XApplication::getInstance()->setAnimationInterval(1.0/20);
	XApplication::getInstance()->runInBack();
}


#endif