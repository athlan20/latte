#include "XScheduler.h"

#include "../base/global.h"

#include <boost/function.hpp>


void XScheduler::setInterval(float interval)
{
	_interval = interval;
}

void XScheduler::update()
{
	//先调用 默认函数
	if (!_defaultPerform.empty())
	{
		//for (const auto &function : _defaultPerform)
		//{
		//	function();
		//}
	}
	// Testing size is faster than locking / unlocking.
	// And almost never there will be functions scheduled to be called.
	//if (!_functionsToPerform.empty()) {
	//	_performMutex.lock();
	//	// fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
	//	auto temp = _functionsToPerform;
	//	_functionsToPerform.clear();
	//	_performMutex.unlock();
	//	for (const auto &function : temp) {
	//		function();
	//	}
	//}
}

void XScheduler::addDefaultPerform(const boost::function<void()> &function)
{
	this->_defaultPerform.push_back(function);
}

void XScheduler::performFunctionInLatteThread(const boost::function<void()> &function)
{
	//_performMutex.lock();

	//_functionsToPerform.push_back(function);

	//_performMutex.unlock();
}

XScheduler::XScheduler()
{

}


XScheduler::~XScheduler()
{
}
