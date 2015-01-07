#ifndef __XUTILSFORMATTER_H_
#define __XUTILSFORMATTER_H_

#include "../base/macros.h"
#include <string>
#include <sstream>

class XUtilsFormatter
{
public:
	X_DLL XUtilsFormatter();
	X_DLL ~XUtilsFormatter();

public:
	X_DLL static std::string addslashes(std::string strIn);
	X_DLL static std::string GBK2UTF8(const char *szGbk);
	X_DLL static std::string UTF82GBK(const char *szUtf8);

	template <class T>
	X_DLL static std::string ConvertToString(T value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

};

#endif