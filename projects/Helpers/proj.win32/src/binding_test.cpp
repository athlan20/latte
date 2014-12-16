// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "binding_test.h"
#include "simple_handler.h"

#include "../lib/base/global.h"
#include "../lib/core/XObserver.h"
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

namespace binding_test {

namespace {

const char kTestUrl[] = "http://tests/binding";
const char kTestMessageName[] = "CallNative";

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
		jReader.parse(message, root);
	}
	if (!root.isNull()) {
      // Reverse the string and return.
		std::string funcName = root["funcName"].asString();
		XOBSERVER->notify(funcName);
		callback->Success("success");
		//frame->ExecuteJavaScript("nativeCallJs(\"funcName\")","native",0);
      return true;
    }

    return false;
  }
};

}  // namespace

void CreateMessageHandlers(SimpleHandler::MessageHandlerSet& handlers) {

	//注册方法
	XOBSERVER->listen("selectDirectory", [](std::string str){
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

			}
			CComPtr<IMalloc> pMalloc;
			::SHGetMalloc(&pMalloc);
			pMalloc->Free(pidlSel);   //释放资源1
			pMalloc->Free(pNetHoodIDL);   //释放资源 2
		}
	});

  handlers.insert(new Handler());
}

}  // namespace binding_test
