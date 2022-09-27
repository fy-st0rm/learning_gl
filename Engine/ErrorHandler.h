#pragma once

#include <iostream>

#include "GL/glew.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) ClearError(); \
    x;\
    ASSERT(ErrorLog(#x, __FILE__, __LINE__))

void ClearError();
bool ErrorLog(const char* function, const char* file, int line);
