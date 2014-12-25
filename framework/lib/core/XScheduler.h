#ifndef __XSCHEDULER_H_
#define __XSCHEDULER_H_

#include "../base/macros.h"

#include <vector>
#include <mutex>

class XScheduler
{
public:
	X_DLL XScheduler();
	X_DLL ~XScheduler();

public:
	X_DLL void update();

	/**
	 * ��֤�����߳��е��øú���
	 * @param
	 * @return
	 */
	X_DLL void performFunctionInLatteThread(const std::function<void()> &function);
	
	/**
	 * ����tickˢ�¼��
	 * @param
	 * @return
	 */
	X_DLL void setInterval(float interval);

	/**
	 * ����Ĭ�ϸ��º���
	 * @param
	 * @return
	 */
	X_DLL void addDefaultPerform(const std::function<void()> &function);

protected:
	std::vector<std::function<void()>> _functionsToPerform;
	std::mutex _performMutex;
	std::vector<std::function<void()>> _defaultPerform;
	float _delay;
	float _interval;
};

#endif __XSCHEDULER_H_



