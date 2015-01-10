#ifndef _XUPGRADE_H_
#define _XUPGRADE_H_

#include "../../base/macros.h"
#include "../XDownloader.h"

#include "json/json.h"

#include <boost/functional.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <memory>

class XUpdater
{
public:
	X_DLL XUpdater(const char* packageUrl/* =NULL */, const char* resVersionUrl/* =NULL */, const char* storagePath/* =NULL */);
	X_DLL virtual ~XUpdater();
	
	X_DLL void reset();			//���ð汾
	X_DLL bool checkVersion(std::string localVersionPath = "");	//���汾
	X_DLL std::string getLocalMainVersion(std::string mainVersionUrl = "");		//��ȡ����app�汾��

	X_DLL virtual bool init();
	X_DLL virtual int upgrade(std::string localPackagePath="package\\",std::string localVersionPath = "package\\resource.json",
		boost::function<void(int)> startLoad = NULL,
		boost::function<void(double, double, const std::string &, const std::string &)> progressCall = NULL,
		boost::function<void(const std::string &, const std::string &, const std::string &)> successCall = NULL,
		boost::function<void()> allCompleteCall = NULL,
		boost::function<void(double, double, const std::string &, const std::string &)> errorCall = NULL
		);		//���汾����
	

	X_DLL virtual void onStart();		//��ʼ���ػص����Ѿ��õ��ļ���С�Ѿ������ļ����ܴ�С
	X_DLL virtual void onError();		//����ص�
	X_DLL virtual void onProgress(int percent);		//�������ؽ��Ȼص�
	X_DLL virtual void onSuccess();		//���سɹ�

private:
	void initDownloadDir();	//��������Ŀ¼
	virtual std::string getMainVersion(std::string mainVersionUrl="");		//��ȡ����汾��
	X_DLL virtual int checkPackage(boost::unordered_map<std::string, XDownloader::XDownloadUnit>& units);

private:
	std::string _storagePath;			//�����·��
	std::string _version;				//��ǰ�汾��
	std::string _packageUrl;			//
	std::string _resVersionUrl;			//��Դ�汾�ļ�
	std::string _downloadedVersion;
	std::string _connectionTimeout;
	bool _isDownloading;

	Json::Value  _serverResJson;

	boost::shared_ptr<XDownloader> _downloader;		//����������
};

#endif