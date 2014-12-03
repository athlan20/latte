#ifndef __XLATTE_H_
#define __XLATTE_H_

#include "XScheduler.h"

class XLatte
{
public:
	static XLatte* getInstance();

	void init();
	void update();
	void setAnimationInterval(double interval);
	XScheduler* getScheduler();
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