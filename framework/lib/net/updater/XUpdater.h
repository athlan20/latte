#ifndef _XUPGRADE_H_
#define _XUPGRADE_H_

#include "../../base/macros.h"
#include "../XDownloader.h"

#include "json/json.h"

#include <string>
#include <functional>
#include <memory>

class XUpdater
{
public:
	
	X_DLL XUpdater(const char* packageUrl/* =nullptr */, const char* resVersionUrl/* =nullptr */, const char* storagePath/* =nullptr */);
	X_DLL virtual ~XUpdater();

	X_DLL void reset();			//重置版本
	X_DLL bool checkVersion(std::string localVersionPath = "");	//检查版本
	X_DLL std::string getLocalMainVersion(std::string mainVersionUrl = "");		//获取本地app版本号

	X_DLL virtual bool init();
	X_DLL virtual int upgrade(std::string resourcePath = "resource.json",
		std::function<void(int)> startLoad = nullptr,
		std::function<void(double, double, const std::string &, const std::string &)> progressCall = nullptr,
		std::function<void(const std::string &, const std::string &, const std::string &)> successCall = nullptr,
		std::function<void()> allCompleteCall = nullptr,
		std::function<void(double, double, const std::string &, const std::string &)> errorCall = nullptr
		);		//检查版本更新
	X_DLL virtual int checkPackage();
	X_DLL virtual void onStart();		//开始下载回调，已经拿到文件大小已经所有文件的总大小
	X_DLL virtual void onError();		//错误回调
	X_DLL virtual void onProgress(int percent);		//更新下载进度回调
	X_DLL virtual void onSuccess();		//下载成功

private:
	void initDownloadDir();	//创建下载目录
	virtual std::string getMainVersion(std::string mainVersionUrl="");		//获取网络版本号

private:
	std::string _storagePath;			//保存的路径
	std::string _version;				//当前版本号
	std::string _packageUrl;			//
	std::string _resVersionUrl;			//资源版本文件
	std::string _downloadedVersion;
	std::string _connectionTimeout;
	bool _isDownloading;

	Json::Value  _serverResJson;
	std::vector<std::string> _removeFiles;

	std::shared_ptr<XDownloader> _downloader;		//下载器对象
};

#endif