#ifndef __XLATTE_H_
#define __XLATTE_H_

#include "XScheduler.h"
#include "../base/macros.h"

class XLatte
{
public:
	X_DLL static XLatte* getInstance();

	X_DLL void init();
	X_DLL void update();
	X_DLL void setAnimationInterval(double interval);
	X_DLL XScheduler* getScheduler();
protected:
private:
	static XLatte* _instance;
	XScheduler* _scheduler;

	/* delta time since last tick to main loop */
	float _deltaTime;
	/* whether or not the next delta time will be zero */
	bool _nextDeltaTimeZero;
	/* last time the main loop was updated */
	struct timeval *_lastUpdate;
	/* ¼ä¸ô */
	double _animationInterval;

private:
	void calculateDeltaTime();
};

#endif	//__XLATTE_H_