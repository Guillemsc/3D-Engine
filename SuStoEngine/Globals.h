#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

enum ConsoleTextType;

// Deletes a buffer
#define RELEASE( x )		\
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete x;             \
	  x = nullptr;          \
	    }                   \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )  \
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete[] x;           \
	  x = nullptr;          \
	    }                   \
                            \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

typedef unsigned int uint;
typedef unsigned char uchar;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

#define CONSOLE_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_INFO(format, ...) log(ConsoleTextType::C_T_T_INFO, __FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_SUCCES(format, ...) log(ConsoleTextType::C_T_T_SUCCES, __FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_WARNING(format, ...) log(ConsoleTextType::C_T_T_WARNING, __FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_ERROR(format, ...) log(ConsoleTextType::C_T_T_ERROR, __FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);
void log(ConsoleTextType type, const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD   0.0174532925199432957f
#define RADTODEG   57.295779513082320876f
#define PI		   3.14159265358979323846f
#define TWO_PI	   6.28318530717958647692f
#define HALF_PI	   1.57079632679489661923f
#define QUARTER_PI 0.78539816339744830961f
#define INV_PI	   0.31830988618379067154f
#define INV_TWO_PI 0.15915494309189533576f

