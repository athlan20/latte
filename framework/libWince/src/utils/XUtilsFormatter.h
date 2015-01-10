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
	X_DLL static std::wstring UT2WC(const char* buf);
	X_DLL static std::wstring UT2WC(const std::string& str);
	X_DLL static std::string WC2UT(const wchar_t* buf);
	X_DLL static std::string WC2UT(std::wstring& str);
	X_DLL static std::wstring MB2WC(const char* buf);
	X_DLL static std::string WC2MB(const wchar_t* buf);


	template <class T>
	X_DLL static std::string ConvertToString(T value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

};

#endif