#include "stdafx.h"
#include "lib/base/global.h"
#include "lib/utils/XUtilsFile.h"
#include "lib/net/XDownloader.h"
#include "lib/core/XApplication.h"

#include "json/json.h"

#include <stdio.h>
#include <fstream>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
void f1(int n)
{
	XApplication::getInstance()->setAnimationInterval(1.0 / 30);
	XApplication::getInstance()->run();
	
}
namespace LatteTest
{
	std::shared_ptr<XDownloader> downloader;
	std::shared_ptr<std::thread> t_back;
	const std::string localNetworkAddr = "http://www.latte.com/php/test/package/resource.json";// "http://www.latte.com/php/test/github_xpgod.rar";// "http://116.236.150.110/test/web.zip";
	const std::string localNetworkErrAddr = "http://www.latte.com/php/test/cocos2dx-update-temp-package123.zip";
	const std::string resServerRoot = "http://www.latte.com/php/test/package/";
	const std::string storagePathAddr = "./download_package.zip";
	bool hasAsysFinish;
	bool hasProcessCall;
	bool hasErrorCall;
	double totalDownloaded;
	double hasDownloaded;
	TEST_CLASS(Downloader)
	{
	private:

	public:
		TEST_CLASS_INITIALIZE(init)
		{
			XUtilsFile::init();
			t_back = std::shared_ptr<std::thread>(new std::thread(&XApplication::run, (XApplication::getInstance())));
			t_back->detach();
		}

		TEST_METHOD_INITIALIZE(methodInit)
		{
			downloader = std::shared_ptr<XDownloader>(new XDownloader());
			hasAsysFinish = false;
			hasProcessCall = false;
			hasErrorCall = false;
		}

		TEST_METHOD(downloadSync)
		{
			downloader->downloadSync(localNetworkAddr, storagePathAddr.c_str(), "00001");
			bool isExist = XUtilsFile::isFileExist(storagePathAddr);
			Assert::AreEqual(true, isExist);
		}

		TEST_METHOD(downloadAsync)
		{
			downloader->setSuccessCallback([](const std::string &a, const std::string &b, const std::string &c){
				hasAsysFinish = true;
			});
			downloader->downloadAsync(localNetworkAddr, storagePathAddr.c_str(), "00001");
			while (!hasAsysFinish)
			{
				Sleep(0);
			}
			//Sleep(1000);//等待线程先退出完毕
			if (hasAsysFinish)
			{
				//检查是否下载到文件
				bool isExist = XUtilsFile::isFileExist(storagePathAddr);
				Assert::AreEqual(true, isExist);
			}
			else
			{
				Assert::IsFalse("downloadAsync download error,not finish");
			}
		}

		TEST_METHOD(downloadProcessCall)
		{
			downloader->setProgressCallback([](double a, double b, const std::string &c, const std::string &d){
				hasProcessCall = true;
				totalDownloaded = a;
				hasDownloaded = b;
				XLOGP("hasDownloaded:%d %d",b,a);
			});
			downloader->setSuccessCallback([](const std::string &a, const std::string &b, const std::string &c){
				hasAsysFinish = true;
			});
			downloader->downloadAsync(localNetworkAddr, storagePathAddr.c_str(), "00002");
			while (!hasProcessCall || !hasAsysFinish)
			{
				//if (totalDownloaded && hasDownloaded)
				//{
				//	XLOGP("total download:%f, has downloaded:%f",totalDownloaded,hasDownloaded);
				//}
				Sleep(0);
			}
			if (hasProcessCall && hasAsysFinish)
			{
				//检查是否下载到文件
				bool isExist = XUtilsFile::isFileExist(storagePathAddr);
				Assert::AreEqual(true, isExist);
			}
			else
			{
				Assert::IsFalse("downloadAsync download error,not finish");
			}
		}

		TEST_METHOD(queueDownloadCall)
		{
			Json::Reader jReader;
			Json::Value root;
			std::string data = XUtilsFile::getFileData("resource.json");
			jReader.parse(data,root);
			Json::Value files = root["files"];
			Json::Value fileObj;
			Json::Value::Members member = files.getMemberNames();
			std::unordered_map<std::string, XDownloader::XDownloadUnit> units;
			for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
			{
				std::string fileKey = *iter;
				fileObj = files[fileKey];
				std::string fileMD5 = fileObj["key"].asString();

				XDownloader::XDownloadUnit unit;
				unit.srcUrl = resServerRoot + fileKey;
				unit.storagePath = fileKey;
				units[fileKey] = unit;
			}
			downloader->queueDownloadSync(units);
		}

		TEST_METHOD(errorCall)
		{
			downloader->downloadAsync(localNetworkErrAddr, storagePathAddr.c_str(), "00003");
			XDownloader::Error error;
			downloader->setErrorCallback([&error](const XDownloader::Error & err){
				error = err;
				hasErrorCall = true;
			});
			while (!hasErrorCall)
			{
				Sleep(0);
			}
			if (hasErrorCall)
			{
				XDownloader::ErrorCode code = XDownloader::ErrorCode::URL_NOT_FOUND;
				Assert::IsTrue(error.code == code);
			}
			else
			{
				Assert::IsFalse("downloadAsync download error,not finish");
			}
		}

		TEST_METHOD_CLEANUP(methodClean)
		{
			XUtilsFile::deleteFile(storagePathAddr);
		}

		TEST_CLASS_CLEANUP(clean)
		{
			XApplication::getInstance()->destroy();
		}

	};
}