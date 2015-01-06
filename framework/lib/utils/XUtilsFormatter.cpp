#include "XUtilsFormatter.h"

#include <Windows.h>
#include <stdlib.h>


void XUtilsFormatter::addslashes(std::string& out_str)
{
	/* maximum string length, worst case situation */
	std::string newStr="";
	int length = out_str.size();
	int sourceIndex = 0;
	int newIndex = 0;
	char word = '\0';

	if (length == 0) {
		return;
	}

	while (sourceIndex < length) {
		word = out_str[sourceIndex];
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
	out_str = newStr;
}

// 注释：多字节包括GBK和UTF-8  
void XUtilsFormatter::GBK2UTF8(const char *szGbk, char* szUtf8)
{
	// 先将多字节GBK（CP_ACP或ANSI）转换成宽字符UTF-16  
	// 得到转换后，所需要的内存字符数  
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// 字符数乘以 sizeof(WCHAR) 得到字节数  
	WCHAR *str1 = new WCHAR[sizeof(WCHAR)* n];
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


	delete[]str1;
	str1 = NULL;
}

//UTF-8 GBK  
void XUtilsFormatter::UTF82GBK(const char *szUtf8,char* szGBK)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[sizeof(WCHAR)* n];
	//char *szGbk = new char[sizeof(WCHAR)* n];

	memset(wszGBK, 0, sizeof(WCHAR)* n);
	memset(szGBK, 0, sizeof(WCHAR)* n);

	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);
	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, n, NULL, NULL);

	delete[]wszGBK;
	wszGBK = NULL;
}

XUtilsFormatter::XUtilsFormatter()
{
}


XUtilsFormatter::~XUtilsFormatter()
{
}
