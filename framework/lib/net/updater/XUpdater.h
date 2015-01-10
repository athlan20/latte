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

	X_DLL void reset();			//���ð汾
	X_DLL bool checkVersion(std::string localVersionPath = "");	//���汾
	X_DLL std::string getLocalMainVersion(std::string mainVersionUrl = "");		//��ȡ����app�汾��

	X_DLL virtual bool init();
	X_DLL virtual int upgrade(std::string resourcePath = "resource.json",
		std::function<void(int)> startLoad = nullptr,
		std::function<void(double, double, const std::string &, const std::string &)> progressCall = nullptr,
		std::function<void(const std::string &, const std::string &, const std::string &)> successCall = nullptr,
		std::function<void()> allCompleteCall = nullptr,
		std::function<void(double, double, const std::string &, const std::string &)> errorCall = nullptr
		);		//���汾����
	X_DLL virtual int checkPackage();
	X_DLL virtual void onStart();		//��ʼ���ػص����Ѿ��õ��ļ���С�Ѿ������ļ����ܴ�С
	X_DLL virtual void onError();		//����ص�
	X_DLL virtual void onProgress(int percent);		//�������ؽ��Ȼص�
	X_DLL virtual void onSuccess();		//���سɹ�

private:
	void initDownloadDir();	//��������Ŀ¼
	virtual std::string getMainVersion(std::string mainVersionUrl="");		//��ȡ����汾��

private:
	std::string _storagePath;			//�����·��
	std::string _version;				//��ǰ�汾��
	std::string _packageUrl;			//
	std::string _resVersionUrl;			//��Դ�汾�ļ�
	std::string _downloadedVersion;
	std::string _connectionTimeout;
	bool _isDownloading;

	Json::Value  _serverResJson;
	std::vector<std::string> _removeFiles;

	std::shared_ptr<XDownloader> _downloader;		//����������
};

#endif