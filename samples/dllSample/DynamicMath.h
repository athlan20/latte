#ifndef __DYNAMICMATH_H_
#define __DYNAMICMATH_H_

class DynamicMath
{
public:
	__declspec(dllexport) DynamicMath(void);
	__declspec(dllexport) ~DynamicMath(void);

	static __declspec(dllexport) double staticAdd(double a, double b);//加法
	__declspec(dllexport) double add(double a, double b);//加法

	__declspec(dllexport) void print();
};

#endif