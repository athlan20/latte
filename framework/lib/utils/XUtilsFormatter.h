#ifndef __XUTILSFORMATTER_H_
#define __XUTILSFORMATTER_H_
#include <string>
class XUtilsFormatter
{
public:
	XUtilsFormatter();
	~XUtilsFormatter();

public:
	static std::string addslashes(std::string strIn);
	static std::string GBK2UTF8(const char *szGbk);
	static std::string UTF82GBK(const char *szUtf8);
};

#endif