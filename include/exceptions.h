#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * \brief exceptions namespace contains all custom exception types.
 */
namespace exceptions
{
    /**
     * \brief BaseException is a base class for exceptions which has all constructors of std::exception
     * and defines additional constructor, which get error string as std::string.
     */
    class BaseException : public std::exception
    {
        using std::exception::exception;
        
        public:
            BaseException(const std::string& message) : std::exception(message.c_str())
            { }
    };

    /**
     * \brief GLRecAcquisitionException is an exception to indicate error while generating some resources in OpenGL state machine.
     */
    class GLRecAcquisitionException : public BaseException
    {
        using BaseException::BaseException;
    };

    /**
     * \brief FileException is a base exception to indicate error while working with files or directories.
     */
    class FileException : public BaseException
    {
        using BaseException::BaseException;
    };

    /**
     * \brief FileOpeningException is an exception to indicate error while opening file or directory.
     */
    class FileOpeningException : public FileException
    {
        using FileException::FileException;
    };

    /**
     * \brief FileReadingException is an exception to indicate error while reading file or directory.
     */
    class FileReadingException : public FileException
    {
        using FileException::FileException;
    };
}

#endif
