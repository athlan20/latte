#include "XUtilsFormatter.h"

#include <Windows.h>
#include <stdlib.h>


std::string XUtilsFormatter::addslashes(std::string strIn)
{
	/* maximum string length, worst case situation */
	std::string newStr="";
	int length = strIn.size();
	int sourceIndex = 0;
	int newIndex = 0;
	char word = '\0';

	if (length == 0) {
		return strIn;
	}

	while (sourceIndex < length) {
		word = strIn[sourceIndex];
		switch (word) {
			case '\0':
				newStr += '\\';
				newStr += '0';
				break;
			case '\'':
			case '\"':
			case '\\':
				newStr += '\\';
				/* break is missing *intentionally* */
			default:
				newStr += word;
				break;
			}
			++sourceIndex;
	}
	return newStr;
}

// 注释：多字节包括GBK和UTF-8  
std::string XUtilsFormatter::GBK2UTF8(const char *szGbk)
{
	std::string returnStr = "";
	// 先将多字节GBK（CP_ACP或ANSI）转换成宽字符UTF-16  
	// 得到转换后，所需要的内存字符数  
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// 字符数乘以 sizeof(WCHAR) 得到字节数  
	WCHAR *str1 = new WCHAR[sizeof(WCHAR)* n];
	char *szUtf8 = new char[sizeof(WCHAR)* n];
	// 转换  
	MultiByteToWideChar(CP_ACP,  // MultiByte的代码页Code Page  
		0,            //附加标志，与音标有关  
		szGbk,        // 输入的GBK字符串  
		-1,           // 输入字符串长度，-1表示由函数内部计算  
		str1,         // 输出  
		n             // 输出所需分配的内存  
		);

	// 再将宽字符（UTF-16）转换多字节（UTF-8）  
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_UTF8, 0, str1, -1, szUtf8, n, NULL, NULL);

	returnStr.assign(szUtf8);

	delete[]str1;
	str1 = NULL;
	delete[]szUtf8;
	szUtf8 = NULL;

	return returnStr;
}

//UTF-8 GBK  
std::string XUtilsFormatter::UTF82GBK(const char *szUtf8)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[sizeof(WCHAR)* n];
	char *szGbk = new char[sizeof(WCHAR)* n];
	std::string returnStr = "";

	memset(wszGBK, 0, sizeof(WCHAR)* n);
	memset(szGbk, 0, sizeof(WCHAR)* n);

	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);

	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGbk, n, NULL, NULL);
	returnStr.assign(szGbk);

	delete[]wszGBK;
	wszGBK = NULL;
	delete[]szGbk;
	szGbk = NULL;

	return returnStr;
}

XUtilsFormatter::XUtilsFormatter()
{
}


XUtilsFormatter::~XUtilsFormatter()
{
}
