#include "XUtilsFormatter.h"

#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <string>


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

// ע�ͣ����ֽڰ���GBK��UTF-8  
std::string XUtilsFormatter::GBK2UTF8(const char *szGbk)
{
	std::string returnStr = "";
	// �Ƚ����ֽ�GBK��CP_ACP��ANSI��ת���ɿ��ַ�UTF-16  
	// �õ�ת��������Ҫ���ڴ��ַ���  
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// �ַ������� sizeof(WCHAR) �õ��ֽ���  
	WCHAR *str1 = new WCHAR[sizeof(WCHAR)* n];
	char *szUtf8 = new char[sizeof(WCHAR)* n];
	// ת��  
	MultiByteToWideChar(CP_ACP,  // MultiByte�Ĵ���ҳCode Page  
		0,            //���ӱ�־���������й�  
		szGbk,        // �����GBK�ַ���  
		-1,           // �����ַ������ȣ�-1��ʾ�ɺ����ڲ�����  
		str1,         // ���  
		n             // ������������ڴ�  
		);

	// �ٽ����ַ���UTF-16��ת�����ֽڣ�UTF-8��  
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

std::wstring XUtilsFormatter::UT2WC(const char* buf)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
	if (len == 0) return L"";

	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, &unicode[0], len);

	return std::wstring(&unicode[0]);
}

std::string XUtilsFormatter::WC2UT(const wchar_t* buf)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL);
	if (len == 0) return "";

	std::vector<char> utf8(len);
	WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL);

	return std::string(&utf8[0]);
}

std::wstring XUtilsFormatter::MB2WC(const char* buf)
{
	int len = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
	if (len == 0) return std::wstring(L"");

	std::vector<wchar_t> unicode(len);
	MultiByteToWideChar(CP_ACP, 0, buf, -1, &unicode[0], len);

	return std::wstring(&unicode[0]);
}

std::string XUtilsFormatter::WC2MB(const wchar_t* buf)
{
	int len = WideCharToMultiByte(CP_ACP, 0, buf, -1, NULL, 0, NULL, NULL);
	if (len == 0) return "";

	std::vector<char> utf8(len);
	WideCharToMultiByte(CP_ACP, 0, buf, -1, &utf8[0], len, NULL, NULL);

	return std::string(&utf8[0]);
}

XUtilsFormatter::XUtilsFormatter()
{
}


XUtilsFormatter::~XUtilsFormatter()
{
}