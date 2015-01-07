#include "stdafx.h"
#include "lib/base/global.h"
#include "lib/utils/XUtilsFile.h"
#include "lib/utils/XUtilsMath.h"
#include "lib/net/updater/XUpdater.h"
#include "lib/core/XApplication.h"

#include "json/json.h"

#include <atltime.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <unordered_map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LatteTest
{
	const std::string packageUrl = "http://www.latte.com/php/test/";
	const std::string versionUrl = "http://www.latte.com/php/test/resource.json";
	const std::string versionFilePath = "resource.json";
	const std::string storagePath = "";
	const std::string destVersion = "1.0.0";
	std::shared_ptr<XUpdater> updater;
	std::shared_ptr<std::thread> t_back_updater;

	TEST_CLASS(Updater)
	{
	private:
		
	public:
		TEST_CLASS_INITIALIZE(init)
		{
			updater = std::shared_ptr<XUpdater>(new XUpdater(packageUrl.c_str(), versionUrl.c_str(), storagePath.c_str()));
			XUtilsFile::init();
			t_back_updater = std::shared_ptr<std::thread>(new std::thread(&XApplication::run, (XApplication::getInstance())));
			t_back_updater->detach();
		}

		TEST_METHOD(checkVersion)
		{
			//版本正确
			bool isSame = updater->checkVersion(versionFilePath);
			Assert::AreEqual(true, isSame);
		}

		TEST_METHOD(renameFile)
		{
			//XUtilsFile::renameFile("tempMainVersion", "b");
		}

		TEST_METHOD(updateTestUpgrade)
		{
			srand((unsigned)time(NULL));
			
			bool hasLoaded = false;
			//1. 先随机删除或者更改md5文件
			//拿到本地resource
			std::string localResStr = XUtilsFile::getFileData("resource.json");
			std::vector<std::string> files = XUtilsFile::getFilesInDir(".\\resource\\*");
			Json::Reader jReader;
			Json::Value root;
			jReader.parse(localResStr, root);
			root["version"] = "0.0000";			//让版本过期
			std::unordered_map<std::string, time_t> MD5DiffFiles;
			for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
			{
				int rnd = XUtilsMath::getRand(1, 10);
				if (rnd > 8)
				{
					XUtilsFile::deleteFile(*it);
				}
				else if (rnd > 5)
				{
					root["files"][XUtilsFile::formatPath(*it)] = "1231312321312231";
					WIN32_FIND_DATA fd;
					FindFirstFile(it->c_str(), &fd);
					FILETIME ft = fd.ftLastWriteTime;
					CTime ct(ft);
					time_t modifyTime = ct.GetTime();
					MD5DiffFiles[*it] = modifyTime;
					//root
				}
			}
			localResStr = root.toStyledString();
			XUtilsFile::writeFileData("resource.json", localResStr);

			//2. 再算一下本地文件数量
			files = XUtilsFile::getFilesInDir(".\\resource\\*");
			int localFileSize = files.size();

			//3. 下载服务端的资源文件
			std::shared_ptr<XDownloader> downloader = std::shared_ptr<XDownloader>(new XDownloader());
			downloader->downloadSync(versionUrl,"serverRes.json");
			std::string serverResStr = XUtilsFile::getFileData("serverRes.json");
			jReader.parse(serverResStr, root);
			int serverFileSize = root["files"].size();

			//4. 去更新
			int curDownloadNum = 0;
			int needDownloadNum = updater->upgrade("", 
			[=](int totalNum){
				XLOGP("totalNum:%d", totalNum);
			}, 
			[=](double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId){
				XLOGP("loading:%d %d", (int)nowDownloaded, (int)totalToDownload);
			},
			[=, &hasLoaded, &needDownloadNum, &curDownloadNum](const std::string & url, const std::string & localPathName, const std::string & customId){
				++curDownloadNum;
				if (needDownloadNum == curDownloadNum)
				{

				}

			},
			[=, &hasLoaded](){
				XLOG("loading complete");
				hasLoaded = true;
			}
			);

			while (!hasLoaded){
				continue;
			}

			//5. 在算一下本地文件数量
			files = XUtilsFile::getFilesInDir(".\\resource\\*");
			localFileSize = files.size();
			Assert::AreEqual(localFileSize, serverFileSize);

			//6. 算一下MD5不一样文件的修改时间是否正确
			for (std::unordered_map<std::string, time_t>::iterator it = MD5DiffFiles.begin(); it != MD5DiffFiles.end(); ++it)
			{
				WIN32_FIND_DATA fd;
				FindFirstFile(it->first.c_str(), &fd);
				FILETIME ft = fd.ftLastWriteTime;
				CTime ct(ft);
				time_t modifyTime = ct.GetTime();
				time_t oldModifyTime = it->second;
				
				Assert::IsTrue(modifyTime>oldModifyTime);
			}

			//7. 覆盖掉本地资源文件
			XUtilsFile::renameFile("tempMainVersion","resource.json");

		}

		TEST_CLASS_CLEANUP(clean)
		{
			XUtilsFile::deleteFile("serverRes.json");
			
		}

	};
}