// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#include "simple_handler.h"


#include "include/cef_app.h"

#include <string>
#include <set>

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler,
				  public CefRenderProcessHandler
{

public:
	bool createBrowser(std::string url, HWND hWnd, RECT offsetRect);
	bool destroyBrwoser();
	bool refresh();
	bool executeScript(std::string script);
	bool callScript(std::string scriptMethod);
	CefRefPtr<CefBrowser> getBrowser();

public:
	// Interface for browser delegates. All BrowserDelegates must be returned via
	// CreateBrowserDelegates. Do not perform work in the BrowserDelegate
	// constructor. See CefBrowserProcessHandler for documentation.
	class BrowserDelegate : public virtual CefBase {
	public:
		virtual void OnContextInitialized(CefRefPtr<SimpleApp> app) {}

		virtual void OnBeforeChildProcessLaunch(
			CefRefPtr<SimpleApp> app,
			CefRefPtr<CefCommandLine> command_line) {}

		virtual void OnRenderProcessThreadCreated(
			CefRefPtr<SimpleApp> app,
			CefRefPtr<CefListValue> extra_info) {}
	};
	typedef std::set<CefRefPtr<BrowserDelegate> > BrowserDelegateSet;
	// Interface for renderer delegates. All RenderDelegates must be returned via
	// CreateRenderDelegates. Do not perform work in the RenderDelegate
	// constructor. See CefRenderProcessHandler for documentation.
	class RenderDelegate : public virtual CefBase {
	public:
		virtual void OnRenderThreadCreated(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefListValue> extra_info) {}

		virtual void OnWebKitInitialized(CefRefPtr<SimpleApp> app) {}

		virtual void OnBrowserCreated(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual void OnBrowserDestroyed(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser) {}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler(CefRefPtr<SimpleApp> app) {
			return NULL;
		}

		virtual bool OnBeforeNavigation(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			cef_navigation_type_t navigation_type,
			bool is_redirect) {
			return false;
		}

		virtual void OnContextCreated(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnContextReleased(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) {}

		virtual void OnUncaughtException(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Exception> exception,
			CefRefPtr<CefV8StackTrace> stackTrace) {}

		virtual void OnFocusedNodeChanged(CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node) {}

		// Called when a process message is received. Return true if the message was
		// handled and should not be passed on to other handlers. RenderDelegates
		// should check for unique message names to avoid interfering with each
		// other.
		virtual bool OnProcessMessageReceived(
			CefRefPtr<SimpleApp> app,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) {
			return false;
		}
	};
	typedef std::set<CefRefPtr<RenderDelegate> > RenderDelegateSet;

	SimpleApp();
private:
  // CefApp methods.
  //virtual void OnRegisterCustomSchemes(
  //    CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE;
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()		//关键是这句能够在自己APP里回调到render process相关
      OVERRIDE { return this; }
	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;
	virtual void OnBeforeChildProcessLaunch(
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
		OVERRIDE;
private:
	HWND hWnd;
	CefRefPtr<SimpleHandler> _handler;
	

private:
	// Creates all of the BrowserDelegate objects. Implemented in
	// client_app_delegates.
	static void CreateBrowserDelegates(BrowserDelegateSet& delegates);
	// Set of supported BrowserDelegates. Only used in the browser process.
	BrowserDelegateSet browser_delegates_;
	// Creates all of the RenderDelegate objects. Implemented in
	// client_app_delegates.
	static void CreateRenderDelegates(RenderDelegateSet& delegates);
	// Set of supported RenderDelegates. Only used in the renderer process.
	RenderDelegateSet render_delegates_;
	// CefRenderProcessHandler methods.
	virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
		OVERRIDE;
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;
 private:
  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleApp);

  public:
	  static HWND hwnd;
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
