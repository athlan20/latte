#ifndef __XSCHEDULER_H_
#define __XSCHEDULER_H_

#include <vector>
#include <mutex>

class XScheduler
{
public:
	XScheduler();
	~XScheduler();

public:
	void update();

	/**
	 * 保证在主线程中调用该函数
	 * @param
	 * @return
	 */
	void performFunctionInLatteThread(const std::function<void()> &function);
	
	/**
	 * 设置tick刷新间隔
	 * @param
	 * @return
	 */
	void setInterval(float interval);

	/**
	 * 增加默认更新函数
	 * @param
	 * @return
	 */
	void addDefaultPerform(const std::function<void()> &function);

protected:
	std::vector<std::function<void()>> _functionsToPerform;
	std::mutex _performMutex;
	std::vector<std::function<void()>> _defaultPerform;
	float _delay;
	float _interval;
};

#endif __XSCHEDULER_H_



