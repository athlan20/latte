#include "XDownloader.h"
#include "../base/global.h"
#include "../core/XLatte.h"
#include "../utils/XUtilsFile.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <cstdio>
#include <iostream>
#include <string>

#define TEMP_EXT            ".temp"
#define LOW_SPEED_LIMIT     1L
#define LOW_SPEED_TIME      5L
#define MAX_REDIRS          2
#define DEFAULT_TIMEOUT     5
#define HTTP_CODE_SUPPORT_RESUME    206



void XDownloader::prepareDownload(const std::string &srcUrl, const std::string &storagePath, const std::string &customId, bool resumeDownload, FileDescriptor *fDesc, ProgressData *pData)
{
	boost::shared_ptr<XDownloader> downloader = shared_from_this();
	pData->customId = customId;
	pData->url = srcUrl;
	pData->downloader = downloader;
	pData->downloaded = 0;
	pData->totalToDownload = 0;

	fDesc->fp = NULL;
	fDesc->curl = NULL;

	Error err;
	err.customId = customId;

	// Asserts
	// Find file name and file extension
	std::string _storagePath = XUtilsFile::getAbsolutePath(storagePath);
	unsigned long found = _storagePath.find_last_of("/\\");
	if (found != std::string::npos)
	{
		pData->name = _storagePath.substr(found + 1);
		pData->path = _storagePath.substr(0, found + 1);
	}
	else
	{
		err.code = ErrorCode::INVALID_URL;
		err.message = "Invalid url or filename not exist error: " + srcUrl;
		//if (this->_onError) this->_onError(err);
		return;
	}

	//检查文件夹是否存在
	XUtilsFile::checkDirAndCreate(_storagePath);

	// Create a file to save file.
	const std::string outFileName = _storagePath + TEMP_EXT;
	fDesc->fp = fopen(outFileName.c_str(), "wb");

	if (!fDesc->fp)
	{
		err.code = ErrorCode::CREATE_FILE;
		err.message = "Can not create file";
		return;
	}
}


size_t fileWriteFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = (FILE*)userdata;
	size_t written = fwrite(ptr, size, nmemb, fp);
	return written;
}
// Compare to batchDownloadProgressFunc, this only handles progress information notification
int downloadProgressFunc(XDownloader::ProgressData *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	if (ptr->totalToDownload == 0)
	{
		ptr->totalToDownload = totalToDownload;
	}

	if (ptr->downloaded != nowDownloaded)
	{
		ptr->downloaded = nowDownloaded;
		std::cout << int(ptr->downloaded/ptr->totalToDownload*100) << std::endl;

		XDownloader::ProgressData data = *ptr;
		//XLatte::getInstance()->getScheduler()->performFunctionInLatteThread(()());
		//XLatte::getInstance()->getScheduler()->performFunctionInLatteThread([=](){
		//	//study 为什么这里会有expired
		//	if (!data.downloader.expired())
		//	{
		//		boost::shared_ptr<XDownloader> downloader = data.downloader.lock();
		//		auto callback = downloader->getProgressCallback();
		//		if (callback != nullptr)
		//		{
		//			callback(totalToDownload, nowDownloaded, data.url, data.customId);
		//		}
		//	}
		//	else
		//	{
		//		std::cout << "expired" << std::endl;
		//	}
		//});
	}

	return 0;
}
int XDownloader::download(const std::string &srcUrl, const std::string &customId, const FileDescriptor &fDesc, const ProgressData &data)
{
	boost::weak_ptr<XDownloader> ptr = shared_from_this();
	CURL *curl = curl_easy_init();
	if (!curl)
	{
		this->notifyError("Can not init curl with curl_easy_init");
		//this->notifyError(ErrorCode::CURL_EASY_ERROR, "Can not init curl with curl_easy_init", customId);
		return -1;
	}
	long timeout = this->_connectionTimeout;
	// Download pacakge
	curl_easy_setopt(curl, CURLOPT_URL, srcUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileWriteFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fDesc.fp);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, downloadProgressFunc);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data);
	curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
	//curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)1000);
	if (_connectionTimeout) curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	
	//curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, LOW_SPEED_LIMIT);
	//curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, LOW_SPEED_TIME);

	CURLcode res = curl_easy_perform(curl);

	fclose(fDesc.fp);
	curl_easy_cleanup(curl);

	// This can only be done after fclose
	if (res == CURLE_OK)
	{
		this->finishDownload(data);
		XLatte::getInstance()->getScheduler()->performFunctionInLatteThread(boost::bind(&XDownloader::successCall,this,data));
	}
	else
	{
		this->notifyError(res,data);
	}

	return 0;
}

int XDownloader::finishDownload(const ProgressData &data)
{
	XUtilsFile::renameFile(data.name + TEMP_EXT, data.name, data.path);

	return 0;
}

void XDownloader::downloadAsync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId/* = ""*/)
{
	ProgressData* pData = new ProgressData();
	pData->url = srcUrl;
	pData->path = storagePath;
	pData->customId = customId;
	pData->downloader = shared_from_this();
	CreateThread(NULL,NULL,&XDownloader::doanloadAsyncProc,pData,NULL,NULL);
}

void XDownloader::downloadSync(const std::string &srcUrl, const std::string &storagePath, const std::string &customId/* = ""*/)
{
	FileDescriptor fDesc;
	ProgressData pData;
	prepareDownload(srcUrl, storagePath, customId, false, &fDesc, &pData);
	if (fDesc.fp != NULL)
	{
		download(srcUrl, customId, fDesc, pData);
	}
}

void XDownloader::queueDownloadSync(boost::unordered_map<std::string, XDownloadUnit> units)
{
	boost::unordered_map<std::string, XDownloadUnit>::iterator it = units.begin();
	for (; it != units.end(); ++it)
	{
		this->downloadSync(it->second.srcUrl, it->second.storagePath);
	}
}

void XDownloader::queueDownloadASync(boost::unordered_map<std::string, XDownloadUnit> units)
{
	boost::unordered_map<std::string, XDownloadUnit>::iterator it = units.begin();
	for (; it != units.end(); ++it)
	{
		this->downloadAsync(it->second.srcUrl, it->second.storagePath);
	}
}

DWORD XDownloader::doanloadAsyncProc(LPVOID pParam)
{
	ProgressData* data = (ProgressData*)pParam;
	boost::shared_ptr<XDownloader> downloader = data->downloader;
	downloader->downloadSync(data->url,data->path,data->customId);
	delete data;
	return 0;
}

void XDownloader::notifyError(const std::string &msg)
{
	std::cout << " message:" << msg << std::endl;
}

void XDownloader::notifyError(int curle_code/* = CURLE_OK*/,const ProgressData &data)
{
	Error errorObj;
	errorObj.curle_code = curle_code;
	XLatte::getInstance()->getScheduler()->performFunctionInLatteThread(boost::bind(&XDownloader::errorCall,this,errorObj,data));
	//boost::weak_ptr<XDownloader> ptr = shared_from_this();
	/*XLatte::getInstance()->getScheduler()->performFunctionInLatteThread([=]{
		if (!ptr.expired())
		{
			boost::shared_ptr<XDownloader> downloader = ptr.lock();
			auto errorCall = downloader->getErrorCallback();
			if (errorCall)
			{
				Error err;
				err.curle_code = curle_code;
				switch (err.curle_code)
				{
				case CURLE_FAILED_INIT:
					err.code = ErrorCode::CURL_EASY_ERROR;
					break;
				case CURLE_HTTP_RETURNED_ERROR:
					err.code = ErrorCode::URL_NOT_FOUND;
					break;
				case CURLE_OPERATION_TIMEDOUT:
					err.code = ErrorCode::TIME_OUT;
					break;
				default:
					err.code = ErrorCode::NETWORK;
					break;
				}
				errorCall(err);
			}
		}
	});*/
}

void XDownloader::processCall(const ProgressData & data)
{
	if(this->_progressCall)
	{
		this->_progressCall(data.totalToDownload,data.downloaded,data.url,data.customId);
	}
}
void XDownloader::successCall(const ProgressData & data)
{
	if(data.downloader)
	{
		boost::shared_ptr<XDownloader> downloader = data.downloader;
		SuccessCallback _success = downloader->getSuccessCallback();
		//		boost::shared_ptr<XDownloader> downloader = data.downloader.lock();
		//		auto callback = downloader->getSuccessCallback();
		if(_success)
		{
			_success(data.url,data.name,data.customId);
		}
	}
}
void XDownloader::errorCall(const XDownloader::Error & e,const ProgressData & data)
{
	if(data.downloader)
	{
		boost::shared_ptr<XDownloader> downloader = data.downloader;
		ErrorCallback _error = downloader->getErrorCallback();
		if(_error)
		{
			_error(e);
		}
	}
}

XDownloader::XDownloader() :_connectionTimeout(10)
{

}

XDownloader::~XDownloader()
{

}