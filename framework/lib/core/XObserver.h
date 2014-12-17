#ifndef __XOBSERVER_H_
#define __XOBSERVER_H_

#include "json/json.h"

#include <vector>
#include <mutex>
#include <memory>
#include <map>
#include <string>

#define XOBSERVER XObserver::getInstance()

class XObserver
{
public:
	XObserver();
	~XObserver();

	static XObserver* getInstance();

	bool listen(std::string name,std::function<void(void*)> callBack);
	bool unListen(std::string name);
	bool notify(std::string name, void* param=NULL);
	bool notifyOnce(std::string name, void* param = NULL);
	int getMapSize();

private:
	static XObserver* _instance;

	std::map<std::string, std::function<void(void*)>> keyMap;
};

#endif __XOBSERVER_H_



