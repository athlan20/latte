#ifndef __XUTILSMATH_H_
#define __XUTILSMATH_H_

#include "../base/macros.h"

class XUtilsMath
{
public:
	X_DLL XUtilsMath();
	X_DLL ~XUtilsMath();

public:
	X_DLL static int getRand(int randMin, int randMax);
};

#endif