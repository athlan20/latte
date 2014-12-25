#ifndef __XOBSERVER_H_
#define __XOBSERVER_H_

#include "json/json.h"
#include "../base/macros.h"
#include <vector>
#include <mutex>
#include <memory>
#include <map>
#include <string>

#define XOBSERVER XObserver::getInstance()

class XObserver
{
public:
	X_DLL XObserver();
	X_DLL ~XObserver();

	static X_DLL XObserver* getInstance();

	X_DLL bool listen(std::string name, std::function<void(void*)> callBack);
	X_DLL bool unListen(std::string name);
	X_DLL bool notify(std::string name, void* param = NULL);
	X_DLL bool notifyOnce(std::string name, void* param = NULL);
	X_DLL int getMapSize();

private:
	static XObserver* _instance;

	std::map<std::string, std::function<void(void*)>> keyMap;
};

#endif __XOBSERVER_H_



