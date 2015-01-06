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

// ע�ͣ����ֽڰ���GBK��UTF-8  
void XUtilsFormatter::GBK2UTF8(const char *szGbk, char* szUtf8)
{
	// �Ƚ����ֽ�GBK��CP_ACP��ANSI��ת���ɿ��ַ�UTF-16  
	// �õ�ת��������Ҫ���ڴ��ַ���  
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// �ַ������� sizeof(WCHAR) �õ��ֽ���  
	WCHAR *str1 = new WCHAR[sizeof(WCHAR)* n];
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
