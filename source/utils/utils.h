#ifndef INC_UTILS_UTILS_H_
#define INC_UTILS_UTILS_H_

//#ifdef _DEBUG
void dbg(const char* fmt, ...);
//#else
//#define dbg(...)
//#endif

void fatal(const char* fmt, ...);

bool isPressed(int key);

#endif

