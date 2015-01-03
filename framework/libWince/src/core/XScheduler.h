#ifndef __XSCHEDULER_H_
#define __XSCHEDULER_H_

#include "../base/macros.h"
#include <boost/function.hpp>
#include <vector>


class XScheduler
{
public:
	X_DLL XScheduler();
	X_DLL ~XScheduler();

public:
	X_DLL void update();

	/**
	 * 保证在主线程中调用该函数
	 * @param
	 * @return
	 */
	X_DLL void performFunctionInLatteThread(const boost::function<void()> &function);
	
	/**
	 * 设置tick刷新间隔
	 * @param
	 * @return
	 */
	X_DLL void setInterval(float interval);

	/**
	 * 增加默认更新函数
	 * @param
	 * @return
	 */
	X_DLL void addDefaultPerform(const boost::function<void()> &function);

protected:
	std::vector<boost::function<void()>> _functionsToPerform;
	HANDLE _performMutex; //使用手动重置为无信号状态，初始化时有信号状态
	std::vector<boost::function<void()>> _defaultPerform;
	float _delay;
	float _interval;
};

#endif __XSCHEDULER_H_



