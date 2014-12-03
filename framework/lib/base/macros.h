#ifndef __XMACROS_H_
#define __XMACROS_H_

#include <iostream>

#define COCOS2D_DEBUG 1

#ifndef XASSERT
#define XASSERT(cond, msg) do{						\
if (!(cond)){	\
std::cout << "Assert failed: " << msg << std::endl;	\
_ASSERT(cond);	\
}\
} while (0)

#endif

#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX

#ifndef X_BREAK_IF
#define X_BREAK_IF(cond) if(cond){break;}
#endif	//X_BREAK_IF

#endif

// define supported target platform macro which CC uses.
#define X_PLATFORM_UNKNOWN            0
#define X_PLATFORM_IOS                1
#define X_PLATFORM_ANDROID            2
#define X_PLATFORM_WIN32              3
#define X_PLATFORM_MARMALADE          4
#define X_PLATFORM_LINUX              5
#define X_PLATFORM_BADA               6
#define X_PLATFORM_BLACKBERRY         7
#define X_PLATFORM_MAC                8
#define X_PLATFORM_NACL               9
#define X_PLATFORM_EMSCRIPTEN        10
#define X_PLATFORM_TIZEN             11
#define X_PLATFORM_QT5               12
#define X_PLATFORM_WP8               13
#define X_PLATFORM_WINRT             14

// win32
#if defined(_WIN32) && defined(_WINDOWS)
#undef  X_TARGET_PLATFORM
#define X_TARGET_PLATFORM	X_PLATFORM_WIN32
#endif