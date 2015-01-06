// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "binding_test.h"
#include "helpers.h"
#include "simple_app.h"
#include "simple_handler.h"

#include "../lib/utils/XUtilsFile.h"
#include "../lib/utils/XUtilsFormatter.h"
#include "../lib/base/global.h"
#include "../lib/core/XObserver.h"
#include "../lib/net/updater/XUpdater.h"
#include "../external/md5/md5.h"
#include "../lib/core/XApplication.h"
#include "json/json.h"

#include "include/wrapper/cef_stream_resource_handler.h"
#include "shlobj.h"
#include <atlstr.h>
#include <wtypes.h>  
#include <atlbase.h>  
#include <shlobj.h>  
#include <algorithm>
#include <string>
#include <csignal>

extern CefRefPtr<SimpleApp> app;

namespace binding_test {

namespace {

const char kTestUrl[] = "http://tests/binding";
const char kTestMessageName[] = "CallNative";
std::shared_ptr<XUpdater> updater;

class Callback : public CefBase {
public:
	///
	// Notify the associated JavaScript onSuccess callback that the query has
	// completed successfully with the specified |response|.
	///
	virtual void Success(const CefString& response) = 0;

	///
	// Notify the associated JavaScript onFailure callback that the query has
	// failed with the specified |error_code| and |error_message|.
	///
	virtual void Failure(int error_code, const CefString& error_message) = 0;
};

// Handle messages in the browser process.
class Handler : public CefMessageRouterBrowserSide::Handler{
 public:
  Handler() {}

  // Called due to cefQuery execution in binding.html.
  virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
						  CefRefPtr<CefFrame> frame,
						  int64 query_id,
						  const CefString& request,
						  bool persistent,
						  CefRefPtr<Callback> callback) OVERRIDE{
    // Only handle messages from the test URL.
    const std::string& url = frame->GetURL();
    //if (url.find(kTestUrl) != 0)
    //  return false;

    const std::string& message = request;
	Json::Value root;
	if (message != ""){
		Json::Reader jReader;
		std::string formatterStr = "ab";
		char szGBK[512] = {0};
		XUtilsFormatter::UTF82GBK(message.c_str(), szGBK);
		jReader.parse(szGBK, root);
	}
	if (!root.isNull()) {
      // Reverse the string and return.
		std::string funcName = root["funcName"].asString();
		bool ret = XOBSERVER->notify(funcName, (void*)&(root["funcParam"]));
		if (ret)
		{
			callback->Success("success");
		}
		else
		{
			XLOGP("native has no method call: %s ", funcName.c_str());
		}
		//frame->ExecuteJavaScript("nativeCallJs(\"funcName\")","native",0);
      return true;
    }

    return false;
  }
};

}  // namespace

void CreateMessageHandlers(SimpleHandler::MessageHandlerSet& handlers) 
{
	std::string keyName = "selectDirectory";
	//注册方法
	XOBSERVER->listen(keyName, [](void* str){
		XLOG("selectDirectory");
		BROWSEINFO bi;
		::ZeroMemory(&bi, sizeof(bi));	//将bi结构清零  
		char szSelPath[MAX_PATH];       //被选择文件夹对象名称的缓冲区 
		bi.pszDisplayName = szSelPath;
		LPITEMIDLIST pNetHoodIDL;
		::SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pNetHoodIDL);  //根文件夹为历史文件夹
		bi.pidlRoot = pNetHoodIDL;
		bi.lpszTitle = "选择打包路径";    //提示字符串 
		bi.ulFlags = BIF_EDITBOX |			//显示编辑框  
			//BIF_STATUSTEXT |                     //显示状态文本  
			BIF_RETURNONLYFSDIRS |				//只显示文件夹
			BIF_NEWDIALOGSTYLE |
			BIF_VALIDATE;                       //校验编辑框中的输入   BIF_BROWSEINCLUDEFILES |    //允许选择文件对象  
		LPITEMIDLIST pidlSel = ::SHBrowseForFolder(&bi);    //打开文件夹浏览对话框 
		if (pNetHoodIDL != NULL)
		{
			char m_lpszDefaultDir[MAX_PATH] = { 0 };
			char szPath[MAX_PATH];
			if (SHGetPathFromIDList(pidlSel, szPath))
			{
				Json::Value root;
				Json::Value param;
				Json::FastWriter writer;
				std::string szPathStr = std::string(szPath);
				root["funcName"] = "selectDirectory";
				param["path"] = "";
				XUtilsFormatter::addslashes(szPathStr);
				param["path"] = szPathStr;
				root["param"] = param;
				app->executeScript(writer.write(root));
			}
			CComPtr<IMalloc> pMalloc;
			::SHGetMalloc(&pMalloc);
			pMalloc->Free(pidlSel);		  //释放资源1
			pMalloc->Free(pNetHoodIDL);   //释放资源 2
		}
	});
	keyName = "submitPackage";
	XOBSERVER->listen(keyName, [](void* str){
		XLOG("submitPackage");
		Json::Value* root = (Json::Value*)(str);
		XLOG(root->toStyledString().c_str());
		if ((*root)["srcPath"].isNull())  //|| (*root)["destPath"].isNull())
		{
			CallJSAlert("请选择源文件夹");
			return;
		}
		std::string version = (*root)["version"].isNull() ? "0.001" : (*root)["version"].asString();
		std::string srcPath = (*root)["srcPath"].asString();
		std::string destPath = (*root)["destPath"].isNull() ? srcPath : (*root)["destPath"].asString();

		std::vector<std::string> files;
		XUtilsFile::getFilesInDir(srcPath + "\\*", files);
		std::vector<std::string>::iterator iteratorFile = files.begin();

		Json::Value jsonData;
		Json::Value filesArr;
		Json::Value fileObj;
		Json::Value dataObj;
		jsonData["version"] = version;
		std::string repStr = "";
		MD5 md5;
		for (; iteratorFile != files.end(); ++iteratorFile)
		{
			std::ifstream ifs(iteratorFile->c_str());
			md5.update(ifs);
			repStr = (*iteratorFile).replace(0, srcPath.size() + 1, ""); //+1 是为了过滤掉紧接着的\符号
			XUtilsFile::formatPath(repStr);
			dataObj["md5"] = md5.toString();
			dataObj["size"] = XUtilsFile::getFileSize(srcPath+repStr);
			filesArr[repStr.c_str()] = dataObj;
		}
		jsonData["files"] = filesArr;

		XUtilsFile::writeFileData(destPath + "\\resource.json", jsonData.toStyledString());
	});
	keyName = "update";
	XOBSERVER->listen(keyName, [](void* param){
		XLOG("update");
		Json::Value* root = (Json::Value*)(param);
		std::string updateSrcPath = (*root)["updateSrcPath"].asString()+"\\";
		
		XLatte::getInstance()->getScheduler()->performFunctionInLatteThread([=](){
			int i = 0;
			updater = std::shared_ptr<XUpdater>(new XUpdater("http://www.latte.com/php/test/", "http://www.latte.com/php/test/resource.json", updateSrcPath.c_str()));
			int loadNum = updater->upgrade(updateSrcPath + "./resource.json",
				[=](int totalNum){
				Json::Value dataRoot;
				Json::Value param;
				Json::FastWriter writer;
				dataRoot["funcName"] = "startLoad";
				param["totalNum"] = totalNum;
				dataRoot["param"] = param;
				app->executeScript(writer.write(dataRoot));
			},
				[=](double totalToDownload, double nowDownloaded, const std::string & url, const std::string & customId){
				Json::Value root;
				Json::Value param;
				Json::FastWriter writer;
				root["funcName"] = "onLoading";
				param["nowLoaded"] = nowDownloaded;
				param["totalLoaded"] = totalToDownload;
				param["url"] = url;
				root["param"] = param;
				app->executeScript(writer.write(root));
			},
				[=](const std::string & url, const std::string & localPathName, const std::string & customId){
				Json::Value root;
				Json::Value param;
				Json::FastWriter writer;

				root["funcName"] = "onLoaded";
				param["url"] = url;
				root["param"] = param;
				app->executeScript(writer.write(root));
			});
		});
		//CallJSAlert("更新完成");
	});

	handlers.insert(new Handler());
}

void CallJSAlert(std::string msg)
{
	Json::Value root;
	Json::Value param;
	Json::FastWriter writer;

	root["funcName"] = "showAlert";
	param["msg"] = msg;
	root["param"] = param;
	app->executeScript(writer.write(root));
}

void CallJsLog(std::string msg)
{
	app->getBrowser()->GetMainFrame()->ExecuteJavaScript("console.log(\'"+msg+"\')","native",0);
	//frame->ExecuteJavaScript("nativeCallJs('" + script + "');", frame->GetURL(), 0);
}

}  // namespace binding_test
