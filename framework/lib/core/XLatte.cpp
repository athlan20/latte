#include "XLatte.h"
#include "../base/global.h"
#include "../base/macros.h"
#include "XApplication.h"

#include <math.h>

XLatte* XLatte::_instance = nullptr;

XLatte* XLatte::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new XLatte();
		_instance->init();
	}
	return _instance;
}

XScheduler* XLatte::getScheduler()
{
	return this->_scheduler;
}

void XLatte::init()
{
	this->_scheduler = new XScheduler();
	this->_lastUpdate = new struct timeval;
}

void XLatte::update()
{
	this->calculateDeltaTime();
	this->_scheduler->update();
}

void XLatte::setAnimationInterval(double interval)
{
	_animationInterval = interval;
}

// calculate "global" dt
void XLatte::calculateDeltaTime()
{
	struct timeval now;

	if (gettimeofday(&now, nullptr) != 0)
	{
		XLOG("error in gettimeofday");
		_deltaTime = 0;
		return;
	}

	if (_nextDeltaTimeZero)
	{
		_deltaTime = 0;
		_nextDeltaTimeZero = false;
	}
	else
	{
		_deltaTime = (now.tv_sec - _lastUpdate->tv_sec) + (now.tv_usec - _lastUpdate->tv_usec) / 1000000.0f;
		_deltaTime = MAX(0, _deltaTime);
	}
}