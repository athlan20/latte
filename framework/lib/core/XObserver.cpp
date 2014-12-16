#include "XObserver.h"

XObserver* XObserver::_instance = nullptr;

XObserver::XObserver()
{

}
XObserver::~XObserver()
{

}

XObserver* XObserver::getInstance()
{
	if (!_instance)
	{
		_instance = new XObserver();
	}
	return _instance;
}

bool XObserver::listen(std::string name, std::function<void(std::string)> callBack)
{
	this->keyMap[name] = callBack;
	return true;
}
bool XObserver::unListen(std::string name)
{
	std::map<std::string, std::function<void(std::string)>>::iterator it = this->keyMap.find(name);
	if (it->first!="")
	{
		this->keyMap.erase(it);
		return true;
	}
	return false;
}
bool XObserver::notify(std::string name, std::string param)
{
	std::map<std::string, std::function<void(std::string)>>::iterator it = this->keyMap.find(name);
	if (it->first != "")
	{
		std::function<void(std::string)> callBack = it->second;
		callBack(param);
		return true;
	}
	return false;
}
bool XObserver::notifyOnce(std::string name, std::string param)
{
	bool res = this->notify(name,param);
	this->unListen(name);
	return res;
}
int XObserver::getMapSize()
{
	return this->keyMap.size();
}