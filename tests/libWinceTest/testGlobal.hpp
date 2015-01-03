#ifndef _TESTGLOBAL_H__
#define _TESTGLOBAL_H__
#include "CppUnitLite/Test.h"

SimpleString StringFrom(const std::string& value)
{
	return SimpleString(value.c_str());
}

#endif