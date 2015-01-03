#include "XScheduler.h"

#include "../base/global.h"

#include <boost/function.hpp>


void XScheduler::setInterval(float interval)
{
	_interval = interval;
}

void XScheduler::update()
{
	std::vector<boost::function<void()>>::iterator it;
	//先调用 默认函数
	if (!_defaultPerform.empty())
	{
		it = _defaultPerform.begin();
		for(;it!=_defaultPerform.end();++it)
		{
			(*it)();
		}
	}
	if (!_functionsToPerform.empty()) {
		DWORD dReturn = WaitForSingleObject(_performMutex,INFINITE);
		// fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
		std::vector<boost::function<void()>> temp = _functionsToPerform;
		_functionsToPerform.clear();
		ReleaseMutex(_performMutex);
		
		it = temp.begin();
		for(;it!=temp.end();++it)
		{
			(*it)();
		}
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
	DWORD dReturn = WaitForSingleObject(_performMutex,INFINITE);
	_functionsToPerform.push_back(function);
	ReleaseMutex(_performMutex);


	//_performMutex.lock();

	//_functionsToPerform.push_back(function);

	//_performMutex.unlock();
}

XScheduler::XScheduler()
{
	_performMutex = CreateMutex(NULL,FALSE,L"");
}


XScheduler::~XScheduler()
{
}
