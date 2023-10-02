#ifndef OGLS_EXCEPTIONS_EXCEPTIONS_H
#define OGLS_EXCEPTIONS_EXCEPTIONS_H

#include <exception>
#include <format>
#include <string>

/**
 * \namespace ogls::exceptions
 * \brief exceptions namespace contains all custom exception types.
 */
namespace ogls::exceptions
{
/**
 * \brief BaseException is a base class for exceptions which has all constructors of std::exception
 * and defines additional constructors to get error string as std::string or std::string_view.
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
        explicit BaseException(const std::string& message) noexcept : std::exception{message.c_str()}
        {
        }

        /**
         * \brief Constructs new std::exception and passes message to its constructor.
         *
         * \param message - error message. Must be null-terminated C-style string.
         */
        explicit BaseException(std::string_view message) noexcept : std::exception{message.data()}
        {
        }

};  // class BaseException

//------ OPENGL EXCEPTIONS

/**
 * \brief GLRecAcquisitionException is an exception to indicate error while generating some resources in OpenGL state
 * machine.
 */
class GLRecAcquisitionException : public BaseException
{
    public:
        using BaseException::BaseException;

};  // class GLRecAcquisitionException

//------ FILE EXCEPTIONS

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

//------ WINDOW EXCEPTIONS

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

//------ MATH EXCEPTIONS

/**
 * \brief MathException is a base exception to indicate error while math calculations.
 */
class MathException : public BaseException
{
    public:
        using BaseException::BaseException;

};  // class MathException

/**
 * \brief DivisionByZeroException is an exception caused by division by zero while calculations.
 */
class DivisionByZeroException final : public MathException
{
    public:
        /**
         * \brief Constructs new std::exception and passes message to its constructor.
         *
         * \param mathStatement - math statement, which caused division by zero. Must be null-terminated C-style string.
         */
        explicit DivisionByZeroException(const char* mathStatement) :
            MathException{std::format("Division by zero: {}.", mathStatement)}
        {
        }

        /**
         * \brief Constructs new std::exception and passes message to its constructor.
         *
         * \param mathStatement - math statement, which caused division by zero.
         */
        explicit DivisionByZeroException(const std::string& mathStatement) :
            DivisionByZeroException{mathStatement.c_str()}
        {
        }

        /**
         * \brief Constructs new std::exception and passes message to its constructor.
         *
         * \param mathStatement - math statement, which caused division by zero. Must be null-terminated C-style string.
         */
        explicit DivisionByZeroException(std::string_view mathStatement) : DivisionByZeroException{mathStatement.data()}
        {
        }

};  // class DivisionByZeroException

/**
 * \brief MatrixException is an exception to indicate error while operating on Matrices.
 */
class MatrixException : public MathException
{
    public:
        using MathException::MathException;

};  // class MatrixException

}  // namespace ogls::exceptions

#endif
