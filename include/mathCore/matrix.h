#ifndef OGLS_MATHCORE_MATRIX_H
#define OGLS_MATHCORE_MATRIX_H

#include <format>
#include <functional>
#include <string>

#include "helpers/macros.h"

namespace ogls::mathCore
{
/**
 * \brief Matrix represents a N*M dimensional matrix.
 *
 * Matrix with dimensions equal to 0 can be created only by default constructor.
 * Such Matrix is considered as null Matrix.
 * All other constructors require not zero values of dimensions.
 *
 * Indexes are counted starting from 0.
 */
class Matrix
{
    public:
        /**
         * \brief Matrix::Size represents a dimension of the Matrix or specific index in the Matrix.
         */
        struct Size
        {
                size_t rows    = {0};
                size_t columns = {0};

                /**
                 * \brief Returns a std::string representation of the Size object.
                 */
                operator std::string() const
                {
                    return std::format("Matrix::Size(rows={}, columns={})", rows, columns);
                }

                friend bool operator==(const Size&, const Size&) = default;

        };  // struct Size

    public:
        /**
         * \brief Constructs a null Matrix.
         *
         * The Matrix has size 0x0.
         */
        Matrix() = default;
        OGLS_DEFAULT_COPYABLE_MOVABLE(Matrix)

        /**
         * \brief Constructs a Matrix with the specified number of rows and columns.
         *
         * The exception is thrown if one of dimensions is 0.
         *
         * \param rowsNumber    - a number of rows in the Matrix.
         * \param columnsNumber - a number of columns in the Matrix.
         * \param defaultValue  - a default value of Matrix elements.
         * \throw ogls::exceptions::MatrixException().
         */
        Matrix(size_t rowsNumber, size_t columnsNumber, float defaultValue = {0.0f});

        /**
         * \brief Constructs a Matrix with the specified number of rows and columns.
         *
         * The exception is thrown if one of dimensions is 0.
         *
         * \param size         - a dimension of the Matrix.
         * \param defaultValue - a default value of Matrix elements.
         * \throw ogls::exceptions::MatrixException().
         */
        explicit Matrix(const Size& size, float defaultValue = {0.0f});

        /**
         * \brief Constructs a Matrix with the specified elements.
         *
         * Both dimensions must be more then 0 and all column-vectors must have the same size,
         * otherwise the exception is thrown.
         *
         * \param values - the elements of the Matrix.
         * \throw ogls::exceptions::MatrixException().
         */
        explicit Matrix(const std::vector<std::vector<float>>& values);

        /**
         * \brief Constructs a Matrix with the specified elements.
         *
         * It is assumed that the values are provided in row-major order. If the input vector has more elements then
         * the rowsNumber * columnsNumber, the values out of this range are ignored.
         *
         * The exception is thrown if one of dimensions is 0 or if the size of the input vector is less then
         * the expected size based on the provided dimensions.
         *
         * \param rowsNumber    - a number of rows in the Matrix.
         * \param columnsNumber - a number of columns in the Matrix.
         * \param values        - the elements of the Matrix.
         * \throw ogls::exceptions::MatrixException().
         */
        Matrix(size_t rowsNumber, size_t columnsNumber, const std::vector<float>& values);

        //------ SOME UNARY OPERATIONS

        /**
         * \brief Returns new Matrix as the result of negation of this Matrix.
         */
        Matrix operator-() const;

        /**
         * \brief Returns true if this Matrix is null Matrix or zero-matrix, false otherwise.
         *
         * \see isNullMatrix(), isZeroMatrix().
         */
        bool operator!() const noexcept
        {
            return !bool{*this};
        }

        //------ SOME COMPOUND ASSIGNMENT OPERATIONS

        /**
         * \brief Adds another Matrix to this Matrix.
         *
         * Changes this Matrix by performing addition of elements of m to elements of this Matrix.
         * Both Matrix must have equal dimensionality, otherwise the exception is thrown.
         *
         * \param m - another Matrix to add to this Matrix.
         * \return this Matrix with changed elements.
         * \throw ogls::exceptions::MatricesDimensionalityException().
         */
        Matrix& operator+=(const Matrix& m);

        /**
         * \brief Adds number to all elements of this Matrix.
         *
         * Changes this Matrix by performing addition of the number to all elements of this Matrix.
         *
         * \param num - a number to add to all elements of this Matrix.
         * \return this Matrix with changed elements.
         */
        Matrix& operator+=(float num) noexcept;

        /**
         * \brief Subtracts another Matrix from this Matrix.
         *
         * Changes this Matrix by performing subtraction of elements of m from elements of this Matrix.
         * Both Matrix must have equal dimensionality, otherwise the exception is thrown.
         *
         * \param m - another Matrix to subtract from this Matrix.
         * \return this Matrix with changed elements.
         * \throw ogls::exceptions::MatricesDimensionalityException().
         */
        Matrix& operator-=(const Matrix& m);

        /**
         * \brief Subtracts number from all elements of this Matrix.
         *
         * Changes this Matrix by performing subtraction of the number from all elements of this Matrix.
         *
         * \param num - a number to subtract from all elements of this Matrix.
         * \return this Matrix with changed elements.
         */
        Matrix& operator-=(float num) noexcept;

        /**
         * \brief Multiplies all elements of this Matrix by number.
         *
         * Changes this Matrix by performing multiplication of all elements of this Matrix by number.
         *
         * \param num - a number to multiply elements of this Matrix by.
         * \return this Matrix with changed elements.
         */
        Matrix& operator*=(float num) noexcept;

        /**
         * \brief Divides all elements of this Matrix by number.
         *
         * Changes this Matrix by performing division of all elements of this Matrix by number.
         *
         * \param num - a number to divide elements of this Matrix by.
         * \return this Matrix with changed elements.
         * \throw ogls::exceptions::DivisionByZeroException().
         */
        Matrix& operator/=(float num);

        //------ SOME TYPE CASTING OPERATIONS

        /**
         * \brief Returns boolean representation of the Matrix object.
         *
         * \see isNullMatrix(), isZeroMatrix().
         * \return false if the Matrix is null Matrix or zero-matrix, true otherwise.
         */
        explicit operator bool() const noexcept
        {
            return !(isNullMatrix() || isZeroMatrix());
        }

        /**
         * \brief Returns std::string representation of the Matrix object.
         *
         * \see toFullString().
         */
        explicit operator std::string() const
        {
            return toFullString();
        }

        //------

        /**
         * \brief Retrieves the number of columns in the Matrix.
         *
         * \return the number of columns.
         */
        size_t getColumnCount() const noexcept
        {
            return m_columnsNumber;
        }

        /**
         * \brief Retrieves the number of rows in the Matrix.
         *
         * \return the number of rows.
         */
        size_t getRowCount() const noexcept
        {
            return m_rowsNumber;
        }

        /**
         * \brief Retrieves the value at the specified row and column in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param row    - the row index.
         * \param column - the column index.
         * \return the value at the specified position.
         * \throw std::out_of_range.
         */
        float getValue(size_t row, size_t column) const;

        /**
         * \brief Retrieves the value at the specified position in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param elementPosition - position of the element in the Matrix.
         * \return the value at the specified position.
         * \throw std::out_of_range.
         */
        float getValue(const Size& elementPosition) const;

        /**
         * \brief Checks if the Matrix is an identity matrix (square matrix with ones on the main diagonal
         * and zeros elsewhere).
         *
         * \return true if the Matrix is an identity matrix, false otherwise.
         */
        bool isIdentityMatrix() const noexcept;

        /**
         * \brief Checks if the Matrix is a matrix of ones (matrix where every entry is equal to one).
         *
         * \return true if the Matrix is a matrix of ones, false otherwise.
         */
        bool isMatrixOfOnes() const noexcept;

        /**
         * \brief Checks if the Matrix is a null Matrix (dimensions are 0).
         *
         * \return true if the Matrix is a null Matrix, false otherwise.
         */
        bool isNullMatrix() const noexcept
        {
            return m_rowsNumber == 0 && m_columnsNumber == 0;
        }

        /**
         * \brief Checks if the Matrix is a zero-matrix (all elements are 0.0).
         *
         * \return true if the Matrix is a zero-matrix, false otherwise.
         */
        bool isZeroMatrix() const noexcept;

        /**
         * \brief Executes the provided functor on every element of the Matrix.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of Matrix::Size structure and the value of the current
         * element via second argument. Functor must return new value of the passed element.
         */
        void performOnEvery(std::function<float(Size, float)> functor);

        /**
         * \brief Executes the provided functor on every element of the Matrix without changing the element of Matrix.
         *
         * This method allows to iterate over elements of the Matrix and perform an action,
         * which doesn't change the elements of the Matrix.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of Matrix::Size structure and the value of the current
         * element via second argument.
         */
        void performOnEvery(std::function<void(Size, float)> functor) const;

        /**
         * \brief Sets the value at the specified row and column in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param row    - the row index.
         * \param column - the column index.
         * \param value  - the value to set.
         * \throw std::out_of_range.
         */
        void setValue(size_t row, size_t column, float value);

        /**
         * \brief Sets the value at the specified position in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param elementPosition - position of the element in the Matrix.
         * \param value           - the value to set.
         * \throw std::out_of_range.
         */
        void setValue(const Size& elementPosition, float value);

        /**
         * \brief Returns the dimensionality of the Matrix.
         */
        Size size() const noexcept
        {
            return Size{.rows = m_rowsNumber, .columns = m_columnsNumber};
        }

        /**
         * \brief Returns a std::string representation of the Vector object with all elements and size.
         *
         * \param columnWidth - is a minimum number of characters (digits + dot) of Matrix element to be displayed.
         */
        std::string toFullString(int columnWidth = 6) const;

        /**
         * \brief Returns a std::string representation of the Vector object only with size of the Matrix.
         */
        std::string toSizeOnlyString() const;

    private:
        /**
         * \brief The number of columns.
         */
        size_t             m_columnsNumber = {0};
        /**
         * \brief The elements of Matrix.
         */
        std::vector<float> m_data;
        /**
         * \brief The number of rows.
         */
        size_t             m_rowsNumber = {0};

};  // class Matrix

//------ OPERATIONS ON MATRIX

/**
 * \brief Checks equality of two Matrix.
 *
 * \return true if two Matrix have equal dimensionality and equal elements, false otherwise.
 */
bool operator==(const Matrix& m1, const Matrix& m2) noexcept;

/**
 * \brief Checks if two Matrix are not equal.
 *
 * \return true if two Matrix have different dimensionality or different elements, false otherwise.
 */
inline bool operator!=(const Matrix& m1, const Matrix& m2) noexcept
{
    return !(m1 == m2);
}

/**
 * \brief Adds two Matrix.
 *
 * Both Matrix must have equal dimensionality, otherwise the exception is thrown.
 *
 * \return Matrix, which is the result of addition of two Matrix.
 * \throw ogls::exceptions::MatricesDimensionalityException().
 */
Matrix operator+(const Matrix& m1, const Matrix& m2);

/**
 * \brief Adds number to a Matrix.
 *
 * \param num - a number to add to every element of the Matrix.
 * \param m   - a source Matrix.
 * \return new Matrix, which is a result of addition of the number to every element of Matrix m.
 */
Matrix operator+(float num, const Matrix& m);

/**
 * \brief Adds number to a Matrix.
 *
 * \param m   - a source Matrix.
 * \param num - a number to add to every element of the Matrix.
 * \return new Matrix, which is a result of addition of the number to every element of Matrix m.
 */
inline Matrix operator+(const Matrix& m, float num)
{
    return num + m;
}

/**
 * \brief Subtracts two Matrix.
 *
 * Both Matrix must have equal dimensionality, otherwise the exception is thrown.
 *
 * \return Matrix, which is the result of subtraction of two Matrix.
 * \throw ogls::exceptions::MatricesDimensionalityException().
 */
Matrix operator-(const Matrix& m1, const Matrix& m2);

/**
 * \brief Subtracts the number from the Matrix.
 *
 * \param m   - a source Matrix.
 * \param num - a number to subtract from every element of the Matrix.
 * \return new Matrix, which is a result of subtraction of the number from every element of Matrix m.
 */
Matrix operator-(const Matrix& m, float num);

/**
 * \brief Multiplies two Matrix.
 *
 * The number of columns of the Matrix m1 must be equal to the number of rows of the Matrix m2,
 * otherwise the exception is thrown.
 *
 * \return Matrix, which is the result of multiplication of two Matrix.
 * \throw ogls::exceptions::MatricesDimensionalityException().
 */
Matrix operator*(const Matrix& m1, const Matrix& m2);

/**
 * \brief Multiplies all elements of the Matrix by number.
 *
 * \param num - a number to multiply elements of the Matrix by.
 * \param m   - a source Matrix.
 * \return new Matrix, which is a result of multiplication of every element of Matrix m by number.
 */
Matrix operator*(float num, const Matrix& m);

/**
 * \brief Multiplies all elements of the Matrix by number.
 *
 * \param m   - a source Matrix.
 * \param num - a number to multiply elements of the Matrix by.
 * \return new Matrix, which is a result of multiplication of every element of Matrix m by number.
 */
inline Matrix operator*(const Matrix& m, float num)
{
    return num * m;
}

/**
 * \brief Divides all elements of the Matrix by number.
 *
 * \param m   - a source Matrix.
 * \param num - a number to divide elements of the Matrix by.
 * \return new Matrix, which is a result of division of every element of Matrix m by number.
 * \throw ogls::exceptions::DivisionByZeroException().
 */
Matrix operator/(const Matrix& m, float num);

}  // namespace ogls::mathCore

#endif
