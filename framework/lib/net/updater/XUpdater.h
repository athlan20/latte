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

	

	
	void reset();			//���ð汾
	bool checkVersion(std::string localVersionPath="");	//���汾
	std::string getLocalMainVersion(std::string mainVersionUrl="");		//��ȡ����app�汾��

	virtual bool init();
	virtual void upgrade(std::string resourcePath = "resource.json");		//���汾����
	virtual void onError();		//����ص�
	virtual void onProgress(int percent);		//�������ؽ��Ȼص�
	virtual void onSuccess();		//���سɹ�

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

	std::shared_ptr<XDownloader> _downloader;		//����������
};

#endif