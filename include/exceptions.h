#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * \brief EXCEPTION_DERIVED_FROM_CUSTOM_EXCEPTION is a macros to create derived from custom exception class with
 * two constructors to get exception message as std::string or as C-string.
 */
#define EXCEPTION_DERIVED_FROM_CUSTOM_EXCEPTION(Comment, Derived, Base) \
    Comment\
    class Derived : public Base\
    {\
    public:\
        Derived(const char* const message) : Base(message)\
        { }\
        \
        Derived(const std::string& message) : Base(message.c_str())\
        { }\
    \
    };

/**
 * \brief EXCEPTION_DERIVED_FROM_STD_EXCEPTION is a macros to create derived from std::exception class with
 * two constructors to get exception message as std::string or as C-string.
 */
#define EXCEPTION_DERIVED_FROM_STD_EXCEPTION(Comment, X) \
    EXCEPTION_DERIVED_FROM_CUSTOM_EXCEPTION(Comment, X, std::exception)

namespace exceptions
{
	
    EXCEPTION_DERIVED_FROM_STD_EXCEPTION(
    /** 
	 * \brief GLRecAcquisitionException is an exception to indicate error while generating some resources in OpenGL state machine.
	 */,
    GLRecAcquisitionException)

    EXCEPTION_DERIVED_FROM_STD_EXCEPTION(
    /**
	 * \brief FileException is a base exception to indicate error while working with files or directories.
	 */,
    FileException)

    EXCEPTION_DERIVED_FROM_CUSTOM_EXCEPTION(
    /**
	 * \brief FileOpeningException is an exception to indicate error while opening file or directory.
	 */,
    FileOpeningException, FileException)

    EXCEPTION_DERIVED_FROM_CUSTOM_EXCEPTION(
    /**
	 * \brief FileReadingException is an exception to indicate error while reading file or directory.
	 */,
    FileReadingException, FileException)
}

#endif
