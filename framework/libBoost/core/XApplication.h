#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_

#include "XLatte.h"
#include "../base/macros.h"
#include <windows.h>

class XApplication
{
public:
	X_DLL void run();
	//循环体交给宿主程序,每次循环俩调用process
	X_DLL void process();
	X_DLL void stop();
	X_DLL void init();
	X_DLL void destroy();

	X_DLL XApplication()
	{
	}

	X_DLL ~XApplication()
	{
	}

public:
	X_DLL static XApplication* getInstance();
	X_DLL void setAnimationInterval(double interval);
private:
	static XApplication* _instance;
	bool _stop;
	XLatte* _latte;	
	LARGE_INTEGER       _animationInterval;
	LARGE_INTEGER _nFreq;
	LARGE_INTEGER _nLast;
	LARGE_INTEGER _nNow;
};


#endif	//_XAPPLICATION_H_