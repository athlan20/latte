#ifndef __XOBSERVER_H_
#define __XOBSERVER_H_

#include "json/json.h"

#include <vector>
#include <mutex>
#include <memory>
#include <string>

class XObserver
{
public:
	XObserver();
	~XObserver();

	bool listen(std::string name,std::function<void(std::string,Json::Value json)> callBack);
};

#endif __XOBSERVER_H_



