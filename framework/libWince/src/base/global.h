#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <stdio.h>
#include <wtypes.h>
#include <stdarg.h>
#include <windows.h>
#include <string>

#ifdef _WIN32_WCE
#include <WinSock2.h>
#endif
/**
 *  ����־��
 * @param
 * @return
 */
#define XLOGP(FORMAT,...) XLog::log(FORMAT,__VA_ARGS__)
#define XLOG(FORMAT) XLog::log(FORMAT)
class XLog
{
public:
	static void log(const char* format, ...)
	{
		char logStr[512] = { '\0' };
		va_list args;
		va_start(args, format);
		
		int len = vsprintf(logStr, format, args);

		//std::cout<<logStr<<std::endl;
		va_end(args);
		

	}
protected:

private:
};


inline int gettimeofday(struct timeval * val, struct timezone *)
{
	if (val)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}

#endif