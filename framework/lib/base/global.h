#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <stdio.h>
#include <stdarg.h>
#ifdef WIN32
#include <windows.h>
#endif // WIN32

/**
 *  简单日志类
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

		int len = vsnprintf_s(logStr, sizeof(logStr), format, args);
		strcat_s(logStr, "\n");
#ifdef WIN32
		OutputDebugStringA(logStr);
#else
		printf(logStr, "\n");
#endif // WIN32
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