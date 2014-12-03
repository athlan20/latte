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
	 * ��֤�����߳��е��øú���
	 * @param
	 * @return
	 */
	void performFunctionInLatteThread(const std::function<void()> &function);
	
	/**
	 * ����tickˢ�¼��
	 * @param
	 * @return
	 */
	void setInterval(float interval);

	/**
	 * ����Ĭ�ϸ��º���
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



