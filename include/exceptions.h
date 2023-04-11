#ifndef OGLS_EXCEPTIONS_EXCEPTIONS_H
#define OGLS_EXCEPTIONS_EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * \namespace exceptions
 * \brief exceptions namespace contains all custom exception types.
 */
namespace ogls::exceptions
{
/**
 * \brief BaseException is a base class for exceptions which has all constructors of std::exception
 * and defines additional constructor, which get error string as std::string.
 */
class BaseException : public std::exception
{
    public:
        using std::exception::exception;

        /**
         * \brief Constructs new std::exception and passes message to its constructor.
         *
         * \param message - error message.
         */
        explicit inline BaseException(const std::string& message) : std::exception(message.c_str())
        {
        }

};  // class BaseException

/**
 * \brief GLRecAcquisitionException is an exception to indicate error while generating some resources in OpenGL state machine.
 */
class GLRecAcquisitionException : public BaseException
{
    public:
        using BaseException::BaseException;

};  // class GLRecAcquisitionException

/**
 * \brief FileException is a base exception to indicate error while working with files or directories.
 */
class FileException : public BaseException
{
    public:
        using BaseException::BaseException;

};  // class FileException

/**
 * \brief FileOpeningException is an exception to indicate error while opening file or directory.
 */
class FileOpeningException : public FileException
{
    public:
        using FileException::FileException;

};  // class FileOpeningException

/**
 * \brief FileReadingException is an exception to indicate error while reading file or directory.
 */
class FileReadingException : public FileException
{
    public:
        using FileException::FileException;

};  // class FileReadingException

/**
 * \brief WindowException is a base exception to indicate error from the Window.
 */
class WindowException : public BaseException
{
    public:
        using BaseException::BaseException;

};  // class WindowException

/**
 * \brief WindowInitializationException is an exception to indicate error while Window construction.
 */
class WindowInitializationException : public WindowException
{
    public:
        using WindowException::WindowException;

};  // class WindowInitializationException

}  // namespace ogls::exceptions

#endif
