#include "stdafx.h"

#include "lib/base/global.h"
#include "lib/utils/XUtilsFile.h"
#include "lib/core/XObserver.h"
#include "external/md5/md5.h"
#include "DynamicMath.h"

#include "json/json.h"
#include "curl/curl.h"

#include <functional>

#include <vector>
#include <fstream>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LatteTest
{

	const std::string targetFileName = "jsonTest.txt";
	const std::string targetFileMD5 = "7815b94615bb42cea18fec52ef7d4ecf";
	const std::string curlTestDownloadUrl = "http://gwbnsh.onlinedown.net/down/smjson_576018.rar";
	const std::string curlTestBunchUrls[] = { "http://www.latte.com/php/test/cocos2dx-update-temp-package.zip" };// , "http://wwww.236.xdowns.com/uploadFile/2014/JsonAutocoder.zip"
	
	size_t save_file(void *buffer, size_t size, size_t count, void *user_p)
	{
		return fwrite(buffer, size, count, (FILE *)user_p);
	}

	int downloadProgressFunc(int a,double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
	{
		return nowDownloaded;
	}

	void testBind(int a)
	{
		XLOGP("testBind:%d",a);
	}

	bool listen(std::string name, std::function<void(std::string, Json::Value json)> callBack)
	{
		return false;
	}

	void tsetListen(std::string a, Json::Value json=nullptr)
	{
		XLOG(a.c_str());
	}

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(init)
		{
			XUtilsFile::init();
		}

		TEST_METHOD(libTestDll)
		{
			//int answer1 = DynamicMath::staticAdd(1, 2);
			//Assert::AreEqual(answer1, 3);
			//DynamicMath math1;
			//int answer2 = math1.add(2, 3);
			//Assert::AreEqual(answer2, 5);
		}

		TEST_METHOD(libTestMd5)
		{
			MD5 md5;
			std::ifstream ifs(targetFileName.c_str());
			md5.update(ifs);
			XLOG( md5.toString().c_str());
		}

		TEST_METHOD(libTestCurlHeader)
		{
			CURL* curl = curl_easy_init();
			curl_easy_setopt(curl, CURLOPT_URL, curlTestDownloadUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_HEADER, 1);
			curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
			double contentLength = 0;
			if (curl_easy_perform(curl) == CURLE_OK)
			{
				curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentLength);
				Assert::AreEqual(1, 1);
			}
			else
				Assert::AreEqual(1, 0);

			curl_easy_cleanup(curl);
		}

		TEST_METHOD(libTestMutiCurl)
		{
			return;
			CURLM* muti_curl = curl_multi_init();
			int bunchSize = int(sizeof(curlTestBunchUrls) / sizeof(curlTestBunchUrls[0]));
			std::vector<FILE*> fileArr;
			std::vector<CURL*> curlArr;

			for (int i = 0; i < bunchSize; ++i)
			{
				char temp[16] = {'\0'};
				itoa(i, temp, 10);
				std::string fileName = "file" + std::string(temp);
				FILE* file = nullptr;
				fopen_s(&file,fileName.c_str(), "wb");
				if (!file)
				{
					XLOG("file open error");
					return;
				}
				fileArr.push_back(file);

				CURL* curl = curl_easy_init();
				curlArr.push_back(curl);
				curl_easy_setopt(curl, CURLOPT_URL, curlTestBunchUrls[i].c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &save_file);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
				curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, downloadProgressFunc);
				curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, 123);
				curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
				curl_multi_add_handle(muti_curl, curl);
			}

			int running_handle_count;
			while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(muti_curl, &running_handle_count))
			{
				XLOGP("%d curls running...",running_handle_count);
			}

			while (running_handle_count)
			{
				timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 100;
				int max_fd=2;
				long timeout = 0;
				fd_set fd_read;
				fd_set fd_write;
				fd_set fd_except;

				FD_ZERO(&fd_read);
				FD_ZERO(&fd_write);
				FD_ZERO(&fd_except);
				Sleep(100);
				curl_multi_fdset(muti_curl, &fd_read, &fd_write, &fd_except, &max_fd);
				if (max_fd >= 0)		//在win下，需要先判断，不然会一直是-1的curl bug
				{
					int return_code = select(max_fd + 1, &fd_read, &fd_write, &fd_except, &tv);
					if (SOCKET_ERROR == return_code)
					{
						XLOG("socket error");
						break;
					}
					while (CURLM_CALL_MULTI_PERFORM == curl_multi_perform(muti_curl, &running_handle_count)) {
					}
				}
				else
				{
					curl_multi_perform(muti_curl, &running_handle_count);
				}

			}

			for (std::vector<FILE*>::iterator it = fileArr.begin(); it != fileArr.end(); ++it)
			{
				fclose(*it);
			}
			
			for (std::vector<CURL*>::iterator it = curlArr.begin(); it != curlArr.end(); ++it)
			{
				curl_easy_cleanup(*it);
			}
			curl_multi_cleanup(muti_curl);
		}

		TEST_METHOD(libTestJson)
		{
			return;
			//过滤城市控件的数据文件
			std::string data = "";
			XUtilsFile::getFileData("city.json", data);
			//json 文档
			Json::Reader jReader;
			Json::Value root;
			Json::Value citysArr;
			Json::Value cityObj;
			jReader.parse(data, root);
			citysArr = root["cities"];
			int size = citysArr.size();
			for (int i = 0; i < size; ++i)
			{
				cityObj = citysArr[i];

				//开始过滤
				root["cities"][i].removeMember("countryName");
				root["cities"][i].removeMember("dataVer");
				root["cities"][i].removeMember("groupHotelWeight");
				root["cities"][i].removeMember("hxHotelWeight");
				root["cities"][i].removeMember("jp");
				root["cities"][i].removeMember("lat");
				root["cities"][i].removeMember("lon");
				root["cities"][i].removeMember("opr");
				root["cities"][i].removeMember("py");
				root["cities"][i].removeMember("type");
				root["cities"][i].removeMember("vWeight");
				root["cities"][i].removeMember("weight");

			}

			Json::FastWriter writer;
			std::string json_file = writer.write(root);
			XUtilsFile::writeFileData("./city.html", json_file);
			//XLOGP("%d", (root["cities"].size()));
		}

		TEST_METHOD(libTestDir)
		{

			std::vector<std::string> files;
			XUtilsFile::getFilesInDir(".\\resource\\*",files);
			std::vector<std::string>::iterator iteratorFile = files.begin();
			for (; iteratorFile != files.end(); ++iteratorFile)
			{
				XLOG(iteratorFile->c_str());
			}
		}

		TEST_METHOD(libTestBuildVersionJson)
		{
			std::vector<std::string> files;
			XUtilsFile::getFilesInDir(".\\resource\\*",files);
			std::vector<std::string>::iterator iteratorFile = files.begin();

			Json::Value root;
			Json::Value filesArr;
			Json::Value fileObj;
			root["version"] = "0.0001";
			MD5 md5;
			for (; iteratorFile != files.end(); ++iteratorFile)
			{
				std::ifstream ifs(iteratorFile->c_str());
				md5.update(ifs);
				std::string formatUrl = *iteratorFile;
				XUtilsFile::formatPath(formatUrl);
				filesArr[formatUrl.c_str()] = md5.toString();
			}
			root["files"] = filesArr;

			XUtilsFile::writeFileData("resource.json",root.toStyledString());
		}

		TEST_METHOD(libTestObserver)
		{

		}

		TEST_CLASS_CLEANUP(clean)
		{

		}

	};
}