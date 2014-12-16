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

	bool listen(std::string name,std::function<void(std::string)> callBack);
	bool unListen(std::string name);
	bool notify(std::string name, std::string param="");
	bool notifyOnce(std::string name, std::string param = "");
	int getMapSize();

private:
	static XObserver* _instance;

	std::map<std::string, std::function<void(std::string)>> keyMap;
};

#endif __XOBSERVER_H_



