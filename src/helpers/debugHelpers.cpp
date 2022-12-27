#include "helpers/debugHelpers.h"

#include <glad/glad.h>
#include <iostream>

void clearGlError()
{
	while (glGetError());
}

bool checkAndLogGLErrors(const char* file, const char* function, int line)
{
	bool isErrorRaised = false;

	while (auto errorCode = glGetError())
	{
		std::cerr << "[OpenGL error]: code 0x" << std::hex << errorCode << std::dec
			<< " in function " << function << ", in file " << file << ", at line " << line << std::endl;

		isErrorRaised = true;
	}

	return isErrorRaised;
}
