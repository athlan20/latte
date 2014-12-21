#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_

#include "XLatte.h"
#include <windows.h>

class XApplication
{
public:
	void run();
	//循环体交给宿主程序,每次循环俩调用process
	void process();
	void stop();
	void init();
	void destroy();

	XApplication()
	{
	}

	~XApplication()
	{
	}

public:
	static XApplication* getInstance();
	void setAnimationInterval(double interval);
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