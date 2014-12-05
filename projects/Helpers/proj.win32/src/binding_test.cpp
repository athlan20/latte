// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "binding_test.h"
#include "simple_handler.h"
#include <algorithm>
#include <string>

#include "include/wrapper/cef_stream_resource_handler.h"

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

    const std::string& message_name = request;
    if (message_name.find(kTestMessageName) == 0) {
      // Reverse the string and return.
      std::string result = message_name.substr(sizeof(kTestMessageName));
      std::reverse(result.begin(), result.end());
	  callback->Success(result);
	  frame->ExecuteJavaScript("setTimeout(function(){alert(a)},1)","native",0);
      
      return true;
    }

    return false;
  }
};

}  // namespace

void CreateMessageHandlers(SimpleHandler::MessageHandlerSet& handlers) {
  handlers.insert(new Handler());
}

}  // namespace binding_test
