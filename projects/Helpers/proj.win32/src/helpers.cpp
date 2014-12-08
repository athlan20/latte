// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <windows.h>

#include "Resource.h"
#include "simple_app.h"
#include "simple_handler.h"
#include "include/cef_sandbox_win.h"
#include "util.h"

#include "base/global.h"
#include "utils/XUtilsFile.h"

#include "include/cef_app.h"

#include <vector>
#include <list>


// Set to 0 to disable sandbox support.
#define CEF_ENABLE_SANDBOX 0
#define WEBAPP_PATH ".\\..\\..\\webapp\\app\\index.html"

#if CEF_ENABLE_SANDBOX
// The cef_sandbox.lib static library is currently built with VS2010. It may not
// link successfully with other VS versions.
#pragma comment(lib, "cef_sandbox.lib")
#endif

#define MAX_LOADSTRING 100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT  24

HINSTANCE hInst;   // current instance
TCHAR szTitle[MAX_LOADSTRING];  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name
TCHAR szOSRWindowClass[MAX_LOADSTRING];  // the OSR window class name
char szWorkingDir[MAX_PATH];  // The current working directory
UINT uFindMsg;  // Message identifier for find events.
HWND hFindDlg = NULL;  // Handle for the find dialog.
CefRefPtr<SimpleApp> app;


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd;

	hInst = hInstance;  // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam) {

	static HWND backWnd = NULL, forwardWnd = NULL, reloadWnd = NULL,
		stopWnd = NULL, editWnd=NULL, executeWnd = NULL;
	static WNDPROC editWndOldProc = NULL;

	// Static members used for the find dialog.
	static FINDREPLACE fr;
	static WCHAR szFindWhat[80] = { 0 };
	static WCHAR szLastFindWhat[80] = { 0 };
	static bool findNext = false;
	static bool lastMatchCase = false;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	// Callback for the main window
	switch (message) {
		case WM_CREATE: {
			SimpleApp::hwnd = hWnd;
			//加一点按钮
			RECT rect;
			int x = 0;
			GetClientRect(hWnd, &rect);
			reloadWnd = CreateWindow("BUTTON", "Refresh",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, 0, 80,
				30, hWnd, (HMENU)IDC_NAV_RELOAD,hInst, 0);

			//input
			editWnd = CreateWindow("EDIT", 0,
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |
				ES_AUTOVSCROLL | ES_AUTOHSCROLL,
				100, 0, 200, 20, hWnd, (HMENU)IDC_INPUT_SCRIPT, hInst, 0);
			reloadWnd = CreateWindow("BUTTON", "Execute",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,300, 0, 80,
				30, hWnd, (HMENU)IDC_NAV_EXECUTE,hInst, 0);

			//editWndOldProc =
			//	reinterpret_cast<WNDPROC>(GetWindowLongPtr(editWnd, GWLP_WNDPROC));
			//SetWindowLongPtr(editWnd, GWLP_WNDPROC,
			//	reinterpret_cast<LONG_PTR>(WndProc));

			rect.top = 35;

			// Creat the new child browser window
			//CefWindowInfo info;
			//CefBrowserSettings settings;
			//CefRefPtr<SimpleHandler> handler(new SimpleHandler());
			//CefBrowserHost::CreateBrowser(info, handler.get(),
			//	"http://www.baidu.com", settings, NULL);
			int buffSize = 512;
			char buffer[512] = { '\0' };
			char** lppPart = { NULL };
			//char* openUrl = "file:///";

			::GetFullPathName(WEBAPP_PATH, buffSize, buffer, lppPart);
			std::string openUrl = "file:///" + std::string(buffer);
			openUrl = XUtilsFile::formatPath(openUrl);
			app->createBrowser(openUrl.c_str(), hWnd, rect);
			return 0;
		}

		case WM_COMMAND: {
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			case IDC_NAV_RELOAD:
			{
				app->refresh();			
				break;
			}
				case IDC_NAV_EXECUTE:
				{
					//HWND hWndEdit = ::GetDlgItem(hWnd, IDC_NAV_EXECUTE);
					char sztextC[256];
					GetDlgItemText(hWnd, IDC_INPUT_SCRIPT, sztextC, 256);
					//app->callScript(std::string(sztextC));
					app->getBrowser()->GetMainFrame()->LoadURL(sztextC);
					break;
				}
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		case WM_PAINT:
			//一定要一下两句调用,不然会死循环
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;
			break;

		case WM_SETFOCUS:
			return 0;

		case WM_SIZE:
		{
			SimpleApp::hwnd = hWnd;
			HWND cefHwnd = app->getBrowser()->GetHost()->GetWindowHandle();
			if ( wParam != SIZE_MINIMIZED )//窗体大小发生变动。处理函数resize
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				rect.top = 35;
				
				SetWindowPos(cefHwnd, NULL, rect.left, rect.top,
					rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		}
		case WM_ERASEBKGND:
			break;

		case WM_ENTERMENULOOP:
			break;

		case WM_EXITMENULOOP:
			break;

		case WM_CLOSE:
			app->destroyBrwoser();
			
			//CefShutdown();
			//DestroyWindow(hWnd);
			// Allow the close.
			break;
		case WM_QUIT:
			break;
		case WM_DESTROY:
			//CefQuitMessageLoop();
			// Quitting CEF is handled in ClientHandler::OnBeforeClose().
			//CefQuitMessageLoop();
			//DestroyWindow(hWnd); //has execute by cef
			//CefShutdown();
			//PostQuitMessage(0);
			
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYJNI));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MYJNI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



// Entry point function for all processes.
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	void* sandbox_info = NULL;

#if CEF_ENABLE_SANDBOX
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	// Provide CEF with command-line arguments.
	CefMainArgs main_args(hInstance);

	// SimpleApp implements application-level callbacks. It will create the first
	// browser instance in OnContextInitialized() after CEF has initialized.
	app = new SimpleApp();
	
	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

	// Specify CEF global settings here.
	CefSettings settings;

#if !CEF_ENABLE_SANDBOX
	settings.no_sandbox = true;
#endif
	//settings.multi_threaded_message_loop = false;
	// Initialize CEF.
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYJNI, szWindowClass, MAX_LOADSTRING);
	//LoadString(hInstance, IDS_OSR_WIDGET_CLASS, szOSRWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;

	// Run the CEF message loop. This will block until CefQuitMessageLoop() is
	// called.
	CefRunMessageLoop();

	//MSG msg;
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	// Allow processing of find dialog messages.
	//	if (hFindDlg && IsDialogMessage(hFindDlg, &msg))
	//		continue;
	//	XLOG("msg");
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	// Shut down CEF.
	CefShutdown();
	//消息
	//MSG msg;
	//
	////处理消息，进行消息循环
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	return 0;
}

