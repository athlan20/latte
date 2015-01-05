#ifndef _XDOWNLOADER_H_
#define _XDOWNLOADER_H_

#include "../base/macros.h"
#include "curl/curl.h"

#include <boost/unordered_map.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <memory>
#include <string>

class XDownloader:public boost::enable_shared_from_this<XDownloader>
{

public:
	X_DLL XDownloader();
	X_DLL ~XDownloader();

public:
	X_DLL struct ProgressData
	{
		boost::shared_ptr<XDownloader> downloader;
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
	X_DLL enum ErrorCode
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

	typedef boost::function<void(const XDownloader::Error &)> ErrorCallback;
	typedef boost::function<void(double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId)> ProgressCallback;
	typedef boost::function<void(const std::string & url, const std::string & localPathName, const std::string & customId)> SuccessCallback;

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
	X_DLL void queueDownloadSync(boost::unordered_map<std::string, XDownloadUnit> units); //队列加载
	X_DLL void queueDownloadASync(boost::unordered_map<std::string, XDownloadUnit> units);	//队列异步加载
	void X_DLL setErrorCallback(const ErrorCallback &callback) { _errorCall = callback; };
	void X_DLL setProgressCallback(const ProgressCallback &callback) { _progressCall = callback; };
	void X_DLL setSuccessCallback(const SuccessCallback &callback) { _successCall = callback; };
	X_DLL ErrorCallback getErrorCallback() const { return _errorCall; };
	X_DLL ProgressCallback getProgressCallback() const { return _progressCall; };
	X_DLL SuccessCallback getSuccessCallback() const { return _successCall; };

	void processCall(const ProgressData & data);
	void successCall(const ProgressData & data);
	void errorCall(const XDownloader::Error & e,const ProgressData & data);
private:
	void prepareDownload(const std::string &srcUrl, const std::string &storagePath, const std::string &customId, bool resumeDownload, FileDescriptor *fDesc, ProgressData *pData);
	int download(const std::string &srcUrl, const std::string &customId, const FileDescriptor &fDesc, const ProgressData &data);
	int finishDownload(const ProgressData &data);

	void notifyError(const std::string &msg);
	void notifyError(int curle_code/* = CURLE_OK*/,const ProgressData &data);

	ProgressCallback _progressCall;
	ErrorCallback _errorCall;
	SuccessCallback _successCall;

	static DWORD doanloadAsyncProc(LPVOID pParam);


private:
	long _connectionTimeout;
};

#endif // !_XDOWNLOADER_H_



