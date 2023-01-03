#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

namespace exceptions
{
	class GLRecAcquisitionException : public std::exception
	{
		public:
			GLRecAcquisitionException(const char* const message) : std::exception(message)
			{ }

			GLRecAcquisitionException(const std::string& message) : std::exception(message.c_str())
			{ }

	};
}

#endif
