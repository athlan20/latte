#ifndef _XUPGRADE_H_
#define _XUPGRADE_H_

#include "../XDownloader.h"

#include "json/json.h"

#include <string>
#include <functional>
#include <memory>

class XUpdater
{
public:
	XUpdater(const char* packageUrl/* =nullptr */, const char* resVersionUrl/* =nullptr */, const char* storagePath/* =nullptr */);
	virtual ~XUpdater();

	

	
	void reset();			//重置版本
	bool checkVersion(std::string localVersionPath="");	//检查版本
	std::string getLocalMainVersion(std::string mainVersionUrl="");		//获取本地app版本号

	virtual bool init();
	virtual void upgrade(std::string resourcePath = "resource.json");		//检查版本更新
	virtual void onError();		//错误回调
	virtual void onProgress(int percent);		//更新下载进度回调
	virtual void onSuccess();		//下载成功

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

	std::shared_ptr<XDownloader> _downloader;		//下载器对象
};

#endif