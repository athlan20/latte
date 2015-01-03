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
	 * ��֤�����߳��е��øú���
	 * @param
	 * @return
	 */
	X_DLL void performFunctionInLatteThread(const boost::function<void()> &function);
	
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
	X_DLL void addDefaultPerform(const boost::function<void()> &function);

protected:
	std::vector<boost::function<void()>> _functionsToPerform;
	HANDLE _performMutex; //ʹ���ֶ�����Ϊ���ź�״̬����ʼ��ʱ���ź�״̬
	std::vector<boost::function<void()>> _defaultPerform;
	float _delay;
	float _interval;
};

#endif __XSCHEDULER_H_



