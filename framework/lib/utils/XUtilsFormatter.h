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
	X_DLL static void addslashes(std::string& out_str);
	X_DLL static void GBK2UTF8(const char *szGbk, char* szUtf8);
	X_DLL static void UTF82GBK(const char *szUtf8, char* szGBK);

	template <class T>
	X_DLL static void ConvertToString(T value,std::string& out_str) {
		std::stringstream ss;
		ss << value;
		out_str = ss.str();
	}

};

#endif