// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "include/cef_render_process_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "include/cef_keyboard_handler.h"

#include <list>
#include <set>

class SimpleHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
					  public CefKeyboardHandler,
                      public CefLoadHandler{
 public:
  SimpleHandler();
  ~SimpleHandler();

  typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE{
	  return this;
  }

  // CefDisplayHandler methods:
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods:
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  //virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE{
	 // return this;
  //}
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	  CefProcessId source_process,
	  CefRefPtr<CefProcessMessage> message)
	  OVERRIDE;

  // keyboard event handler
  virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	  const CefKeyEvent& event,
	  CefEventHandle os_event,
	  bool* is_keyboard_shortcut) OVERRIDE;

  virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
	  const CefKeyEvent& event,
	  CefEventHandle os_event) OVERRIDE;

  CefRefPtr<CefBrowser> getBrowser();

  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  bool IsClosing() const { return is_closing_; }

protected:
	// Set of Handlers registered with the message router.
	MessageHandlerSet message_handler_set_;
	// Handles the browser side of query routing. The renderer side is handled
	// in client_renderer.cpp.
	CefRefPtr<CefMessageRouterBrowserSide> message_router_;
	// Create all CefMessageRouterBrowserSide::Handler objects. They will be
	// deleted when the ClientHandler is destroyed.
	static void CreateMessageHandlers(MessageHandlerSet& handlers);

 private:
  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
  BrowserList browser_list_;
  CefRefPtr<CefBrowser> _browser;

  bool is_closing_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
