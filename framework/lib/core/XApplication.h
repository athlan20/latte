#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_

#include "XLatte.h"
#include <windows.h>

class XApplication
{
public:
	void run();
	//ѭ���彻����������,ÿ��ѭ��������process
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