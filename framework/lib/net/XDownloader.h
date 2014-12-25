#ifndef _XDOWNLOADER_H_
#define _XDOWNLOADER_H_

#include "../base/macros.h"
#include "curl/curl.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

class XDownloader:public std::enable_shared_from_this<XDownloader>
{

public:
	X_DLL XDownloader();
	X_DLL ~XDownloader();

public:
	X_DLL struct ProgressData
	{
		std::weak_ptr<XDownloader> downloader;
		std::string customId;
		std::string url;
		std::string path;
		std::string name;
		double downloaded;
		double totalToDownload;
	};

	X_DLL struct FileDescriptor
	{
		FILE *fp;
		void *curl;
	};

	//enum
	X_DLL enum class ErrorCode
	{
		CREATE_FILE,

		NETWORK,

		CURL_UNINIT,

		CURL_MULTI_ERROR,

		CURL_EASY_ERROR,

		URL_NOT_FOUND,

		TIME_OUT,

		INVALID_URL,

		INVALID_STORAGE_PATH
	};

	X_DLL struct Error
	{
		ErrorCode code;
		int curlm_code;
		int curle_code;
		std::string message;
		std::string customId;
		std::string url;
	};

	X_DLL struct XDownloadUnit
	{
		std::string srcUrl;
		std::string storagePath;
		std::string customId;
		bool resumeDownload;
	};

	typedef std::function<void(const XDownloader::Error &)> ErrorCallback;
	typedef std::function<void(double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId)> ProgressCallback;
	typedef std::function<void(const std::string & url, const std::string & localPathName, const std::string & customId)> SuccessCallback;

protected:

	struct StreamData
	{
		long offset;
		long total;
		unsigned char *buffer;
	};

public:
	X_DLL void downloadAsync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId = "default");
	X_DLL void downloadSync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId = "default");
	X_DLL void queueDownloadSync(std::unordered_map<std::string, XDownloadUnit> units); //队列加载
	X_DLL void queueDownloadASync(std::unordered_map<std::string, XDownloadUnit> units);	//队列异步加载
	void X_DLL setErrorCallback(const ErrorCallback &callback) { _errorCall = callback; };
	void X_DLL setProgressCallback(const ProgressCallback &callback) { _progressCall = callback; };
	void X_DLL setSuccessCallback(const SuccessCallback &callback) { _successCall = callback; };
	X_DLL ErrorCallback getErrorCallback() const { return _errorCall; };
	X_DLL ProgressCallback getProgressCallback() const { return _progressCall; };
	X_DLL SuccessCallback getSuccessCallback() const { return _successCall; };
private:
	void prepareDownload(const std::string &srcUrl, const std::string &storagePath, const std::string &customId, bool resumeDownload, FileDescriptor *fDesc, ProgressData *pData);
	int download(const std::string &srcUrl, const std::string &customId, const FileDescriptor &fDesc, const ProgressData &data);
	int finishDownload(const ProgressData &data);

	void notifyError(const std::string &msg);
	void notifyError(const std::string &msg/* ="" */, const std::string &customId/* ="" */, int curle_code/* = CURLE_OK*/, int curlm_code/* = CURLM_OK*/);

	ProgressCallback _progressCall;
	ErrorCallback _errorCall;
	SuccessCallback _successCall;


private:
	int _connectionTimeout;
};

#endif // !_XDOWNLOADER_H_



