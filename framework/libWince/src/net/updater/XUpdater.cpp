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

#define DOWNLOAD_FIEL	"download"	//���غ󱣴���ļ�����

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

std::string XUpdater::getLocalMainVersion(std::string mainVersionUrl)		//��ȡ����Ŀ¼
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
	downloader->downloadSync(mainVersionUrl.c_str(),this->_storagePath+"tempMainVersion");
	//json �ĵ�
	Json::Value root;
	Json::Reader jReader;
	std::string data = XUtilsFile::getFileData(this->_storagePath + "tempMainVersion");
	
	if (data != "")
	{
		jReader.parse(data, root);
		this->_serverResJson = root;
		std::string version = root["version"].asString();
		return version;
	}
	return "";
}

/************************************************************************/
/* ���Ȼص�                                                             */
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

int XUpdater::upgrade(std::string resourcePath,
	boost::function<void(int)> startLoad,
	boost::function<void(double, double, const std::string &, const std::string &)> progressCall,
	boost::function<void(const std::string &, const std::string &, const std::string &)> successCall,
	boost::function<void()> allCompleteCall,
	boost::function<void(double, double, const std::string &, const std::string &)> errorCall)
{
	resourcePath = resourcePath == "" ? "resource.json" : resourcePath;
	bool isSame = this->checkVersion(resourcePath);
	int loadedNum = 0;
	std::vector<std::string> removeFiles;
	if (!isSame)
	{
		Json::Reader jReader;
		Json::Value root;
		std::string data = XUtilsFile::getFileData(resourcePath);
		jReader.parse(data, root);
		Json::Value localJson = root["files"];
		
		Json::Value serverJson = this->_serverResJson["files"];
		Json::Value::Members member = serverJson.getMemberNames();
		boost::unordered_map<std::string, XDownloader::XDownloadUnit> units;
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
		{
			std::string serverFileKey = *iter;
			std::string serverFileMD5 = serverJson[serverFileKey].asString();
			if (localJson[serverFileKey].isNull()
				|| localJson[serverFileKey].asString().compare(serverFileMD5) != 0
				|| !XUtilsFile::isFileExist(serverFileKey))	//�����ڻ��߲�һ��
			{
				XDownloader::XDownloadUnit unit;
				unit.srcUrl = this->_packageUrl + serverFileKey;
				unit.storagePath = this->_storagePath + serverFileKey;
				units[serverFileKey] = unit;
			}
		}
		//��鱾���ļ��ǲ�����Ҫɾ��
		member = localJson.getMemberNames();
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
		{
			std::string localFileKey = *iter;
			std::string localFileMD5 = serverJson[localFileKey].asString();
			if (serverJson[localFileKey].isNull()
				&& XUtilsFile::isFileExist(localFileKey))	//serverû�У����Ǳ�����
			{
				removeFiles.push_back(localFileKey);
			}
		}

		loadedNum = units.size();
		if (loadedNum > 0)
		{
			if (startLoad)
			{
				startLoad(loadedNum);
			}
			int curLoadedNum = 0;
			this->_downloader = boost::shared_ptr<XDownloader>(new XDownloader());
			//this->_downloader->setProgressCallback([=](double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId){
			//	//this->
			//	//XLOGP("totalToDownload:%f,nowDownloaded:%f",totalToDownload,nowDownloaded);
			//	if (progressCall)
			//	{
			//		progressCall(totalToDownload, nowDownloaded,url,customId);
			//	}
			//});
			//this->_downloader->setSuccessCallback([=, &curLoadedNum](const std::string & url, const std::string & localPathName, const std::string & customId){
			//	//XLOGP("url:%s success", url.c_str());
			//	if (successCall)
			//	{
			//		successCall(url, localPathName, customId);
			//	}
			//	++curLoadedNum;
			//	if (curLoadedNum == loadedNum)
			//	{
			//		if (allCompleteCall)
			//		{
			//			allCompleteCall();
			//		}
			//	}
			//});
			//������Ҫ���̳߳�ȥ,��Ȼ�Ῠס�����߳�
			
			//boost::thread t = boost::thread(&XDownloader::queueDownloadSync, this->_downloader, units);
			//t.detach();
			//this->_downloader->queueDownloadSync(units);
			//this->_downloader->queueDownloadASync(units);
			//����ʱ�򲻸����������ظ�����
			//XUtilsFile::renameFile("tempMainVersion", "resource.json", this->_storagePath);
			//auto tDelete = std::thread([=, &removeFiles](){
			//	if (removeFiles.size() > 0){
			//		std::vector<std::string>::iterator it = removeFiles.begin();
			//		for (; it != removeFiles.end(); ++it)
			//		{
			//			XUtilsFile::deleteFile(*it);
			//		}

			//	}
			//});
			//tDelete.detach();

		}
		
	}
	return loadedNum;
}