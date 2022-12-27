#ifndef DEBUG_HELPERS_H
#define DEBUG_HELPERS_H

#define ASSERT(x) if (x) __debugbreak();

#define GLCall(x) clearGlError();\
	x;\
	ASSERT(checkAndLogGLErrors(__FILE__, #x, __LINE__));

void clearGlError();

bool checkAndLogGLErrors(const char* file, const char* function, int line);

#endif
