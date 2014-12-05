// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "simple_app.h"

#include <string>
#include <set>

#include "simple_handler.h"
#include "util.h"
#include "client_renderer.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#include "util.h"  // NOLINT(bui


SimpleApp::SimpleApp() {
	//::MessageBoxA(SimpleApp::hwnd, "test", "test", MB_OK);
}


void SimpleApp::OnContextInitialized() {
	CreateBrowserDelegates(browser_delegates_);

	// Register cookieable schemes with the global cookie manager.
	CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager();
	ASSERT(manager.get());
	//manager->SetSupportedSchemes(cookieable_schemes_);

	BrowserDelegateSet::iterator it = browser_delegates_.begin();
	for (; it != browser_delegates_.end(); ++it)
		(*it)->OnContextInitialized(this);
}

void SimpleApp::OnBeforeChildProcessLaunch(
	CefRefPtr<CefCommandLine> command_line) {
	BrowserDelegateSet::iterator it = browser_delegates_.begin();
	for (; it != browser_delegates_.end(); ++it)
		(*it)->OnBeforeChildProcessLaunch(this, command_line);
}
void SimpleApp::OnRenderProcessThreadCreated(
	CefRefPtr<CefListValue> extra_info) {
	BrowserDelegateSet::iterator it = browser_delegates_.begin();
	for (; it != browser_delegates_.end(); ++it)
		(*it)->OnRenderProcessThreadCreated(this, extra_info);
}
void SimpleApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) {
	CreateRenderDelegates(render_delegates_);

	RenderDelegateSet::iterator it = render_delegates_.begin();
	for (; it != render_delegates_.end(); ++it)
		(*it)->OnRenderThreadCreated(this, extra_info);
}
void SimpleApp::OnWebKitInitialized() {
	//HANDLE hFile = ::CreateFile(TEXT("CreateFileDemo.txt"), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
	RenderDelegateSet::iterator it = render_delegates_.begin();
	for (; it != render_delegates_.end(); ++it)
		(*it)->OnWebKitInitialized(this);
	// Define the extension contents.
	std::string extensionCode =
		"var test;"
		"if (!test)"
		"  test = {};"
		"(function() {"
		"  test.myval = 'My Value!';"
		"})();";

	// Register the extension.
	CefRegisterExtension("v8/test", extensionCode, NULL);
}
void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefV8Context> context) {
	RenderDelegateSet::iterator it = render_delegates_.begin();
	for (; it != render_delegates_.end(); ++it)
		(*it)->OnContextCreated(this, browser, frame, context);

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	// Create a new V8 string value. See the "Basic JS Types" section below.
	CefRefPtr<CefV8Value> str = CefV8Value::CreateString("My Value!");

	// Add the string to the window object as "window.myval". See the "JS Objects" section below.
	object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);


}

bool SimpleApp::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message) {
	ASSERT(source_process == PID_BROWSER);

	bool handled = false;

	RenderDelegateSet::iterator it = render_delegates_.begin();
	for (; it != render_delegates_.end() && !handled; ++it) {
		handled = (*it)->OnProcessMessageReceived(this, browser, source_process,
			message);
	}

	return handled;
}

bool SimpleApp::createBrowser(std::string url, HWND hWnd, RECT offsetRect)
{
	this->hWnd = hWnd;
	CefRefPtr<SimpleHandler> handler(new SimpleHandler());
	CefWindowInfo info;
	info.style |= WS_VISIBLE;
	info.style |= WS_MAXIMIZE;
	CefBrowserSettings settings;
	info.SetAsChild(hWnd, offsetRect);

	this->browser = CefBrowserHost::CreateBrowserSync(info, handler.get(), url, settings, NULL);
	
	return true;
}

bool SimpleApp::executeScript(std::string script)
{
	CefRefPtr<CefFrame> frame = this->browser->GetMainFrame();
	frame->ExecuteJavaScript("alert('ExecuteJavaScript works!');", frame->GetURL(), 0);

	return true;
}

bool SimpleApp::callScript(std::string scriptMethod)
{
	return true;
}

CefRefPtr<CefBrowser> SimpleApp::getBrowser()
{
	return this->browser;
}

bool SimpleApp::destroyBrwoser()
{
	this->browser = NULL;

	return true;
}

bool SimpleApp::refresh()
{
	this->browser->Reload();
	return true;
}

//void SimpleApp::OnContextInitialized() {
//  REQUIRE_UI_THREAD();

//  // Information used when creating the native window.
//  CefWindowInfo window_info;
//
//#if defined(OS_WIN)
//  // On Windows we need to specify certain flags that will be passed to
//  // CreateWindowEx().
//  window_info.SetAsPopup(NULL, "cefsimple");
//#endif
//
//  // SimpleHandler implements browser-level callbacks.
//  CefRefPtr<SimpleHandler> handler(new SimpleHandler());
//
//  // Specify CEF browser settings here.
//  CefBrowserSettings browser_settings;
//
//  std::string url;
//
//  // Check if a "--url=" value was provided via the command-line. If so, use
//  // that instead of the default URL.
//  CefRefPtr<CefCommandLine> command_line =
//      CefCommandLine::GetGlobalCommandLine();
//  url = command_line->GetSwitchValue("url");
//  if (url.empty())
//	  url = "file:///./samples/ctrip/myCtrip.htm";
//
//  // Create the first browser window.
//  CefBrowserHost::CreateBrowser(window_info, handler.get(), url,
//                                browser_settings, NULL);
//}

HWND SimpleApp::hwnd = NULL;