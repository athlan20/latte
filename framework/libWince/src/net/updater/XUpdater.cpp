#include "XUpdater.h"
#include "../../../config.h"
#include "../../base/global.h"
#include "../../base/macros.h"
#include "../XDownloader.h"
#include "../../utils/XUtilsFile.h"

#include "json/json.h"
#include <iostream>
#include <cstdio>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#define DOWNLOAD_FIEL	"download"	//下载后保存的文件夹名

XUpdater::XUpdater(const char* packageUrl/* =nullptr */, const char* resVersionUrl/* =nullptr */, const char* storagePath/* =nullptr */) :
_packageUrl(packageUrl),
_resVersionUrl(resVersionUrl),
_storagePath(storagePath)
{

}

XUpdater::~XUpdater()
{

}

bool XUpdater::init()
{
	initDownloadDir();

	return true;
}

bool XUpdater::checkVersion(std::string localVersionPath)
{
	std::string serverVer = this->getMainVersion();
	std::string localVer = this->getLocalMainVersion(localVersionPath);
	
	bool isSame = false;
	XASSERT(serverVer!="","serverVer get empty");
	if (serverVer!="" || localVer!="")
	{
		if (localVer.compare(serverVer) == 0)
		{
			isSame = true;
		}
	}
	return isSame;
}

std::string XUpdater::getLocalMainVersion(std::string mainVersionUrl)		//获取本地目录
{
	mainVersionUrl = mainVersionUrl == "" ? "resource.json" : mainVersionUrl;
	size_t size = 0;
	std::string versionStr = "";
	std::string versionData = XUtilsFile::getFileData(mainVersionUrl);
	if (versionData!="")
	{
		Json::Value root;
		Json::Reader jReader;
		jReader.parse(versionData, root);
		versionStr = root["version"].asString();
	}
	
	return versionStr;
}

void XUpdater::initDownloadDir()
{

}

std::string XUpdater::getMainVersion(std::string mainVersionUrl)
{
	mainVersionUrl = mainVersionUrl == "" ? this->_resVersionUrl : mainVersionUrl;
	boost::shared_ptr<XDownloader> downloader = boost::shared_ptr<XDownloader>(new XDownloader);
	downloader->downloadSync(mainVersionUrl.c_str(),this->_storagePath + "resource.json");
	//json 文档
	Json::Value root;
	Json::Reader jReader;
	std::string data = XUtilsFile::getFileData(this->_storagePath + "resource.json");
	
	if (data != "")
	{
		jReader.parse(data, root);
		this->_serverResJson = root;
		std::string version = root["version"].asString();
		return version;
	}
	return "";
}

int XUpdater::checkPackage(boost::unordered_map<std::string, XDownloader::XDownloadUnit>& units)
{
	if (this->_serverResJson.isNull())
	{
		return -1;
	}
	size_t serverFileSize = 0;
	size_t fileSize = 0;
	Json::Value fileData;
	Json::Value rootFiles = this->_serverResJson["files"];
	int workPathSize = this->_storagePath.size();
	boost::unordered_map<std::string, XDownloader::XDownloadUnit>::iterator it = units.begin();
	for (; it != units.end(); ++it)
	{
		std::string serverFileKey = it->second.storagePath.substr(workPathSize,INT_MAX);
		fileData = rootFiles[serverFileKey];
		std::string serverFileMD5 = fileData["key"].asString();
		if (!XUtilsFile::isFileExist(it->second.storagePath))	//不存在或者不一样
		{
			return -2;
		}
		serverFileSize = fileData["size"].asInt();
		fileSize = XUtilsFile::getFileSize(it->second.storagePath);
		if (serverFileSize != fileSize)
		{
			return -3;
		}
	}
	return 0;
}

/************************************************************************/
/* 进度回调                                                             */
/************************************************************************/
void XUpdater::onStart()
{

}
void XUpdater::onError()
{

}
void XUpdater::onProgress(int percent)
{

}
void XUpdater::onSuccess()
{
}

void XUpdater::reset()
{

}

int XUpdater::upgrade(std::string localPackagePath,std::string localVersionPath,
	boost::function<void(boost::unordered_map<std::string, XDownloader::XDownloadUnit>& )> startLoad,
	boost::function<void(double, double, const std::string &, const std::string &)> progressCall,
	boost::function<void(const std::string &, const std::string &, const std::string &)> successCall,
	boost::function<void()> allCompleteCall,
	boost::function<void(double, double, const std::string &, const std::string &)> errorCall)
{
	localVersionPath = localVersionPath == "" ? "resource.json" : localVersionPath;
	bool isSame = this->checkVersion(localVersionPath);
	int loadedNum = 0;
	std::vector<std::string> removeFiles;
	if (!isSame)
	{
		Json::Reader jReader;
		Json::Value root;
		std::string data = XUtilsFile::getFileData(localVersionPath);
		jReader.parse(data, root);
		Json::Value localJson = root["files"];
		
		Json::Value serverJson = this->_serverResJson["files"];
		Json::Value::Members member = serverJson.getMemberNames();
		Json::Value dataObj;
		boost::unordered_map<std::string, XDownloader::XDownloadUnit> units;
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
		{
			std::string serverFileKey = *iter;
			dataObj =  serverJson[serverFileKey];
			std::string serverFileMD5 = dataObj["key"].asString();
			dataObj = localJson[serverFileKey];
			if (dataObj.isNull()
				|| dataObj["key"].asString().compare(serverFileMD5) != 0
				|| !XUtilsFile::isFileExist(localPackagePath+serverFileKey))	//不存在或者不一样
			{
				XDownloader::XDownloadUnit unit;
				unit.srcUrl = this->_packageUrl + serverFileKey;
				unit.storagePath = this->_storagePath + serverFileKey;
				units[serverFileKey] = unit;
			}
		}
		//检查本地文件是不是需要删除
		member = localJson.getMemberNames();
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
		{
			std::string localFileKey = *iter;
			dataObj =  localJson[localFileKey];
			std::string localFileMD5 = dataObj["key"].asString();
			if (serverJson[localFileKey].isNull()
				&& XUtilsFile::isFileExist(localPackagePath+localFileKey))	//server没有，但是本地有
			{
				removeFiles.push_back(localFileKey);
			}
		}

		loadedNum = units.size();
		if (loadedNum > 0)
		{
			if (startLoad)
			{
				startLoad(units);
			}
			int curLoadedNum = 0;
			this->_downloader = boost::shared_ptr<XDownloader>(new XDownloader());
			this->_downloader->setProgressCallback(progressCall);
			this->_downloader->setSuccessCallback(successCall);

			this->_downloader->queueDownloadSync(units);
			int ret = checkPackage(units);
			if(ret==0)
			{
				XUtilsFile::moveFile(XUtilsFile::getRelativePath("tmpPackage\\"),XUtilsFile::getRelativePath("package\\"));
				if (allCompleteCall)
				{
					allCompleteCall();
				}
			}
		}
		
	}
	return loadedNum;
}