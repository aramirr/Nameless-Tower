#ifndef INC_MCV_PLATFORM_
#define INC_MCV_PLATFORM_

// Global settings
#define _CRT_SECURE_NO_WARNINGS     // Don't warn about using fopen..
#define WIN32_LEAN_AND_MEAN         // Reduce the size of things included in windows.h
#define _USE_MATH_DEFINES           // M_PI M_PI_2

// C/C++
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstdarg>
#include <cstdint>        // uint32_t
#include <algorithm>

#include <vector>
#include <string>
#include <map>

// Windows/OS Platform
#define NOMINMAX                    // To be able to use std::min without windows problems
#include <windows.h>
#include <d3d11.h>

// Engine
#include "render/render.h"
#include "render/vertex_declarations.h"
#include "utils/utils.h"
// ...

#endif
