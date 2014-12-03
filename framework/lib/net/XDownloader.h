#ifndef _XDOWNLOADER_H_
#define _XDOWNLOADER_H_

#include "curl/curl.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

class XDownloader:public std::enable_shared_from_this<XDownloader>
{

public:
	XDownloader();
	~XDownloader();

public:
	struct ProgressData
	{
		std::weak_ptr<XDownloader> downloader;
		std::string customId;
		std::string url;
		std::string path;
		std::string name;
		double downloaded;
		double totalToDownload;
	};

	struct FileDescriptor
	{
		FILE *fp;
		void *curl;
	};

	//enum
	enum class ErrorCode
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

	struct Error
	{
		ErrorCode code;
		int curlm_code;
		int curle_code;
		std::string message;
		std::string customId;
		std::string url;
	};

	struct XDownloadUnit
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
	void downloadAsync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId = "default");
	void downloadSync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId = "default");
	void queueDownloadSync(std::unordered_map<std::string, XDownloadUnit> units); //╤сап╪сть
    void setErrorCallback(const ErrorCallback &callback) { _errorCall = callback; };
    void setProgressCallback(const ProgressCallback &callback) { _progressCall = callback; };
    void setSuccessCallback(const SuccessCallback &callback) { _successCall = callback; };
    ErrorCallback getErrorCallback() const { return _errorCall; };
    ProgressCallback getProgressCallback() const { return _progressCall; };
    SuccessCallback getSuccessCallback() const { return _successCall; };
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



