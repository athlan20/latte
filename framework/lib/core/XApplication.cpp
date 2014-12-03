#include "XApplication.h"
#include "XLatte.h"
#include "../base/global.h"

XApplication* XApplication::_instance = nullptr;

XApplication* XApplication::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new XApplication();
		_instance->init();
	}
	return _instance;
}

void XApplication::init()
{
	this->_stop = false;
	this->_latte = XLatte::getInstance();
}

void XApplication::setAnimationInterval(double interval)
{
	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	_animationInterval.QuadPart = (LONGLONG)(interval * nFreq.QuadPart);
	XLatte::getInstance()->setAnimationInterval(interval);
}
void XApplication::run()
{
	// Main message loop:
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nLast);
	XLOG("app run");
	while (true)
	{
		QueryPerformanceCounter(&nNow);
		if (this->_stop)			//study 作为子线程的循环，需要做好退出逻辑，不然主线程退出后，会出问题
		{
			break;
		}
		if (nNow.QuadPart - nLast.QuadPart > _animationInterval.QuadPart)
		{
			nLast.QuadPart = nNow.QuadPart;

			this->_latte->update();
		}
		else
		{
			Sleep(0);
		}
	}
	XLOG("app end");
}
void XApplication::stop()
{
	this->_stop = true;
}
void XApplication::destroy()
{
	this->stop();
}