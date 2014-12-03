#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_

#include "XLatte.h"
#include <windows.h>

class XApplication
{
public:
	void run();
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
};


#endif	//_XAPPLICATION_H_