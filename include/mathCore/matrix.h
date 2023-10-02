#ifndef OGLS_MATHCORE_MATRIX_H
#define OGLS_MATHCORE_MATRIX_H

#include <array>
#include <iomanip>
#include <optional>
#include <sstream>

#include "exceptions.h"
#include "floats.h"
#include "mathCore/baseMatrix.h"

/**
 * \brief Adds an accessor to the Matrix element on the position.
 */
#define OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(RequiredI, RequiredJ, RequiredN, RequiredM, PassedN, PassedM) \
    /**                                                                                                   \
     * \brief Provides an access to the element of the Matrix on the position [RequiredI][RequiredJ].     \
     */                                                                                                   \
    template<typename = EnableElementAccessor<RequiredN, RequiredM, PassedN, PassedM>>                    \
    constexpr float& m##RequiredN##RequiredM() noexcept                                                   \
    {                                                                                                     \
        return m_data[RequiredI * m_columnsNumber + RequiredJ];                                           \
    }                                                                                                     \
                                                                                                          \
    /**                                                                                                   \
     * \brief Returns the element of the Matrix on the position [RequiredI][RequiredJ].                   \
     */                                                                                                   \
    template<typename = EnableElementAccessor<RequiredN, RequiredM, PassedN, PassedM>>                    \
    constexpr float m##RequiredN##RequiredM() const noexcept                                              \
    {                                                                                                     \
        return m_data[RequiredI * m_columnsNumber + RequiredJ];                                           \
    }


/**
 * \brief Adds a static_assert to check if the Matrix is a square Matrix.
 */
#define OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, functionName)                              \
    static_assert(isSquareMatrixCheck(N, M), "Attempt to use the function " #functionName \
                                             "(), which is defined for a square Matrix, on a non-square Matrix.")

namespace ogls::mathCore
{
/**
 * \brief EnableElementAccessor checks if the accessor to the Matrix element is defined for the Matrix of such
 * dimensionality.
 */
template<size_t RequiredN, size_t RequiredM, size_t PassedN, size_t PassedM>
using EnableElementAccessor = std::enable_if_t<(PassedN >= RequiredN) && (PassedM >= RequiredM)>;

/**
 * \brief IsNotNullMatrix checks if the both dimensions of the Matrix are greater than 0.
 */
template<size_t N, size_t M>
using IsNotNullMatrix = std::enable_if_t<(N > 0) && (M > 0)>;

/**
 * \brief MatrixFunctor checks if the provided functor can be used with Matrix::performOnEvery().
 */
template<typename Functor>
concept MatrixFunctor = requires(Functor obj) {
    // clang-format off
    {obj(std::declval<BaseMatrix::Index>(), 0.0f)} -> std::convertible_to<float>;  // clang-format on
};

/**
 * \brief MatrixConstFunctor checks if the provided functor can be used with Matrix::performOnEvery() const.
 */
template<typename Functor>
concept MatrixConstFunctor = requires(Functor obj) {
    // clang-format off
    {obj(std::declval<BaseMatrix::Index>(), 0.0f)};  // clang-format on
};

/**
 * \brief Checks if the Matrix is a null Matrix (dimensions are 0).
 */
constexpr bool isNullMatrixCheck(size_t n, size_t m) noexcept
{
    return n == 0 && m == 0;
}

/**
 * \brief Checks if the Matrix of size N*M is a square Matrix.
 */
constexpr bool isSquareMatrixCheck(size_t n, size_t m) noexcept
{
    return n != 0 && n == m;
}

/**
 * \brief Matrix represents a N*M dimensional matrix.
 *
 * Matrix with dimensions equal to 0 is considered as null Matrix.
 *
 * Indexes are counted starting from 0.
 *
 * Matrix provides class users with iterators, which can be used with STL algorithms, which don't change
 * the number of elements in provided data container (see BaseMatrix::Iterator).
 *
 * Matrix can be used in constexpr contexts.
 *
 * \code{.cpp}
 * // Usage of range for-loop
 * for (const auto& el : someMatrix)
 * {
 *     std::cout << "element (" << el.i << ", " << el.j << ") = " << el.getValue() << "\n";
 *     el.setValue(el.getValue() + el.i + el.j);
 * }
 * \endcode
 * \code{.cpp}
 * // Usage of STL algorithms
 * std::fill(someMatrix.begin(), someMatrix.end(), 5);
 * std::cout << "Sum of elements: " << std::accumulate(someMatrix.begin(), someMatrix.end(), 0) << std::endl;
 * std::cout << "Number of elements == 5: "
 *                << std::count_if(someMatrix.begin(), someMatrix.end(),
 *                               [](Matrix::iterator::Element el)
 *                               {
 *                                   return el.getValue() == 5;
 *                               })
 *                << std::endl;
 * \endcode
 *
 * \param N - a number of rows in the Matrix.
 * \param M - a number of columns in the Matrix.
 */
template<size_t N, size_t M>
class Matrix : public BaseMatrix
{
        static_assert(N <= 50 && M <= 50,
                      "The dimensionality of the Matrix seems to be too big. "
                      "Check if you passed the correct N and M.");

    public:
        // The following type aliases are the conventions of STL

        /**
         * \brief Iterates over Matrix elements in row-major order from element (0, 0) to element (N-1, M-1)
         * without providing ability to change the referenced Matrix element.
         */
        using const_iterator =
          Iterator<const float, typename std::array<float, N * M>::const_iterator, RowMajorIteratorImpl>;
        /**
         * \brief Iterates over Matrix elements in row-major order from element (0, 0) to element (N-1, M-1).
         */
        using iterator = Iterator<float, typename std::array<float, N * M>::iterator, RowMajorIteratorImpl>;

        // Non-conventional iterator aliases

        /**
         * \brief Iterates over Matrix elements, which belong to the main diagonal of the Matrix ((0, 0), (1, 1) etc.)
         * without providing ability to change the referenced Matrix element.
         */
        using const_diagonal_iterator =
          Iterator<const float, typename std::array<float, N * M>::const_iterator, DiagonalIteratorImpl>;
        /**
         * \brief Iterates over Matrix elements, which belong to the main diagonal of the Matrix ((0, 0), (1, 1) etc.).
         */
        using diagonal_iterator = Iterator<float, typename std::array<float, N * M>::iterator, DiagonalIteratorImpl>;

    public:
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(Matrix)

        /**
         * \brief Constructs a Matrix and fills it with the provided value.
         *
         * \param defaultValue - a default value of Matrix elements.
         */
        constexpr explicit Matrix(float defaultValue = {0.0f}) noexcept : BaseMatrix{N, M}
        {
            for (auto& el : m_data)
            {
                el = defaultValue;
            }
        }

        /**
         * \brief Constructs a Matrix with the specified elements.
         *
         * It is assumed that the values are provided in row-major order.
         *
         * \param values - the elements of the Matrix.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix(const std::array<float, N * M>& values) noexcept : BaseMatrix{N, M}, m_data{values}
        {
        }

        //------ SOME UNARY OPERATIONS

        /**
         * \brief Returns new Matrix as the result of negation of this Matrix.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix operator-() const noexcept
        {
            auto result = Matrix{*this};
            // clang-format off
            result.performOnEvery([](Index, float element) { return -element; });  // clang-format on
            return result;
        }

        /**
         * \brief Returns true if this Matrix is null Matrix or zero-matrix, false otherwise.
         *
         * \see isNullMatrix(), isZeroMatrix().
         */
        constexpr bool operator!() const noexcept
        {
            return !bool{*this};
        }

        //------ SOME COMPOUND ASSIGNMENT OPERATIONS

        /**
         * \brief Adds another Matrix to this Matrix.
         *
         * Changes this Matrix by performing addition of elements of m to elements of this Matrix.
         *
         * \param m - another Matrix to add to this Matrix.
         * \return this Matrix with changed elements.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator+=(const Matrix& m) noexcept
        {
            // clang-format off
            performOnEvery([&](Index pos, float element) { return element + m.getValue(pos); });  // clang-format on
            return *this;
        }

        /**
         * \brief Adds number to all elements of this Matrix.
         *
         * Changes this Matrix by performing addition of the number to all elements of this Matrix.
         *
         * \param num - a number to add to all elements of this Matrix.
         * \return this Matrix with changed elements.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator+=(float num) noexcept
        {
            // clang-format off
            performOnEvery([=](Index, float element) { return element + num; });  // clang-format on
            return *this;
        }

        /**
         * \brief Subtracts another Matrix from this Matrix.
         *
         * Changes this Matrix by performing subtraction of elements of m from elements of this Matrix.
         *
         * \param m - another Matrix to subtract from this Matrix.
         * \return this Matrix with changed elements.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator-=(const Matrix& m) noexcept
        {
            // clang-format off
            performOnEvery([&](Index pos, float element) { return element - m.getValue(pos); });  // clang-format on
            return *this;
        }

        /**
         * \brief Subtracts number from all elements of this Matrix.
         *
         * Changes this Matrix by performing subtraction of the number from all elements of this Matrix.
         *
         * \param num - a number to subtract from all elements of this Matrix.
         * \return this Matrix with changed elements.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator-=(float num) noexcept
        {
            // clang-format off
            performOnEvery([=](Index, float element) { return element - num; });  // clang-format on
            return *this;
        }

        /**
         * \brief Multiplies all elements of this Matrix by number.
         *
         * Changes this Matrix by performing multiplication of all elements of this Matrix by number.
         *
         * \param num - a number to multiply elements of this Matrix by.
         * \return this Matrix with changed elements.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator*=(float num) noexcept
        {
            // clang-format off
            performOnEvery([=](Index, float element) { return element * num; });  // clang-format on
            return *this;
        }

        /**
         * \brief Divides all elements of this Matrix by number.
         *
         * Changes this Matrix by performing division of all elements of this Matrix by number.
         *
         * \param num - a number to divide elements of this Matrix by.
         * \return this Matrix with changed elements.
         * \throw ogls::exceptions::DivisionByZeroException().
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr Matrix& operator/=(float num)
        {
            if (num == 0.0f)
            {
                throw ogls::exceptions::DivisionByZeroException{std::format("{} /= 0.0", toSizeOnlyString())};
            }

            // clang-format off
            performOnEvery([=](Index, float element) { return element / num; });  // clang-format on
            return *this;
        }

        //------ SOME TYPE CASTING OPERATIONS

        /**
         * \brief Returns boolean representation of the Matrix object.
         *
         * \see isZeroMatrix().
         * \return false if the Matrix is null Matrix or zero-matrix, true otherwise.
         */
        constexpr explicit operator bool() const noexcept
        {
            if constexpr (isNullMatrixCheck(N, M))
            {
                return false;
            }
            else
            {
                return !isZeroMatrix();
            }
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

        //------ RANGE STUFF

        /**
         * \brief Returns an Iterator to the beginning.
         */
        constexpr iterator begin() noexcept
        {
            return iterator{m_rowsNumber, m_columnsNumber, m_data.begin()};
        }

        /**
         * \brief Returns const Iterator to the beginning.
         */
        constexpr const_iterator begin() const noexcept
        {
            return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cbegin()};
        }

        /**
         * \brief Returns a DiagonalIterator to the beginning.
         */
        constexpr diagonal_iterator beginDiagonal() noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, beginDiagonal);
            return diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.begin()};
        }

        /**
         * \brief Returns const DiagonalIterator to the beginning.
         */
        constexpr const_diagonal_iterator beginDiagonal() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, beginDiagonal);
            return const_diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.cbegin()};
        }

        /**
         * \brief Returns const Iterator to the beginning.
         */
        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cbegin()};
        }

        /**
         * \brief Returns const DiagonalIterator to the beginning.
         */
        constexpr const_diagonal_iterator cbeginDiagonal() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, cbeginDiagonal);
            return const_diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.cbegin()};
        }

        /**
         * \brief Returns an Iterator to the end.
         */
        constexpr iterator end() noexcept
        {
            return iterator{m_rowsNumber, m_columnsNumber, m_data.end(), true};
        }

        /**
         * \brief Returns const Iterator to the end.
         */
        constexpr const_iterator end() const noexcept
        {
            return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cend(), true};
        }

        /**
         * \brief Returns a DiagonalIterator to the end.
         */
        constexpr diagonal_iterator endDiagonal() noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, endDiagonal);
            return diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.end(), true};
        }

        /**
         * \brief Returns const DiagonalIterator to the end.
         */
        constexpr const_diagonal_iterator endDiagonal() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, endDiagonal);
            return const_diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.cend(), true};
        }

        /**
         * \brief Returns const Iterator to the end.
         */
        constexpr const_iterator cend() const noexcept
        {
            return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cend(), true};
        }

        /**
         * \brief Returns const DiagonalIterator to the end.
         */
        constexpr const_diagonal_iterator cendDiagonal() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, cendDiagonal);
            return const_diagonal_iterator{m_rowsNumber, m_columnsNumber, m_data.cend(), true};
        }

        //------ NAMED ACCESSORS TO THE MOST USED MATRIX ELEMENTS

        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(0, 0, 1, 1, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(0, 1, 1, 2, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(0, 2, 1, 3, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(0, 3, 1, 4, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(1, 0, 2, 1, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(1, 1, 2, 2, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(1, 2, 2, 3, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(1, 3, 2, 4, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(2, 0, 3, 1, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(2, 1, 3, 2, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(2, 2, 3, 3, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(2, 3, 3, 4, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(3, 0, 4, 1, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(3, 1, 4, 2, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(3, 2, 4, 3, N, M)
        OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR(3, 3, 4, 4, N, M)

        //------

        /**
         * \brief Calculates the algebraic complement of the Matrix element.
         *
         * \note The algebraic complement can be calculated only for a square Matrix.
         * \param elementPosition - position of the element in the Matrix.
         * \see isSquareMatrix().
         */
        constexpr float calculateAlgebraicComplement(const Index& elementPosition) const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, calculateAlgebraicComplement);

            const auto sign = float{((elementPosition.rows + elementPosition.columns) % 2 == 0) ? 1.0f : -1.0f};
            return sign * getMinor(elementPosition).calculateDeterminant();
        }

        /**
         * \brief Calculates the determinant of the Matrix.
         *
         * \note The determinant can be calculated only for a square Matrix.
         * \see isSquareMatrix().
         */
        constexpr float calculateDeterminant() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, calculateDeterminant);

            if constexpr (N == 2)
            {
                return (getValue(0, 0) * getValue(1, 1)) - (getValue(0, 1) * getValue(1, 0));
            }
            else if constexpr (N == 1)
            {
                return getValue(0, 0);
            }
            else
            {
                auto result = float{0.0};

                for (auto i = size_t{0}; i < m_rowsNumber; ++i)
                {
                    result += getValue(0, i) * calculateAlgebraicComplement(Index{0, i});
                }

                return result;
            }
        }

        /**
         * \brief Gets the minor of the Matrix element.
         *
         * \note The minor is defined only for a square Matrix.
         * \param elementPosition - position of the element in the Matrix.
         * \see isSquareMatrix().
         */
        constexpr auto getMinor(const Index& elementPosition) const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, getMinor);

            constexpr auto MinorN        = size_t{N > 0 ? N - 1 : 0};
            auto           minor         = Matrix<MinorN, MinorN>{};
            auto           minorIterator = minor.begin();

            for (auto it = begin(); it < end(); ++it)
            {
                // Skip the row, to which the element belongs
                if ((*it).i == elementPosition.rows)
                {
                    it += MinorN;
                    continue;
                }
                if ((*it).j != elementPosition.columns)
                {
                    (*minorIterator) = (*it).getValue();
                    ++minorIterator;
                }
            }

            return minor;
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
        template<typename = IsNotNullMatrix<N, M>>
        constexpr float getValue(size_t row, size_t column) const
        {
            if (row >= m_rowsNumber || column >= m_columnsNumber)
            {
                throw std::out_of_range{
                  formatInvalidElementPositionErrorMessage(m_rowsNumber, m_columnsNumber,
                                                           Index{.rows = row, .columns = column})};
            }
            return m_data[row * m_columnsNumber + column];
        }

        /**
         * \brief Retrieves the value at the specified position in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param elementPosition - position of the element in the Matrix.
         * \return the value at the specified position.
         * \throw std::out_of_range.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr float getValue(const Index& elementPosition) const
        {
            return getValue(elementPosition.rows, elementPosition.columns);
        }

        /**
         * \brief Calculates the inverse Matrix of this Matrix.
         *
         * \note The inverse Matrix can be calculated only for a square Matrix.
         * \see isSquareMatrix().
         * \return the inverse Matrix or std::nullopt, if for this square Matrix the inverse Matrix cannot be calculated.
         */
        constexpr std::optional<Matrix> inverse() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, inverse);

            const auto determinant = calculateDeterminant();
            if (ogls::helpers::isFloatsEqual(determinant, 0.0f))
            {
                return std::nullopt;
            }

            auto additionalMatrix = Matrix{};
            for (const auto& el : *this)
            {
                // Set elements in transpose order in Matrix of algebraic complements
                additionalMatrix.setValue(el.j, el.i, calculateAlgebraicComplement(Index{el.i, el.j}));
            }

            return (1.0f / determinant) * additionalMatrix;
        }

        /**
         * \brief Checks if the Matrix is an identity matrix (square matrix with ones on the main diagonal
         * and zeros elsewhere).
         *
         * \return true if the Matrix is an identity matrix, false otherwise.
         */
        constexpr bool isIdentityMatrix() const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, isIdentityMatrix);

            for (const auto& el : *this)
            {
                if (el.i != el.j)
                {
                    if (ogls::helpers::isFloatsNotEqual(el.getValue(), 0.0f))
                    {
                        return false;  // Non-diagonal elements must be 0
                    }
                }
                else
                {
                    if (ogls::helpers::isFloatsNotEqual(el.getValue(), 1.0f))
                    {
                        return false;  // Diagonal elements must be 1
                    }
                }
            }

            return true;
        }

        /**
         * \brief Checks if the Matrix is inverse Matrix of otherMatrix.
         *
         * \note If this Matrix is inverse Matrix of otherMatrix, otherMatrix is inverse Matrix of this Matrix too.
         * \return true, if this Matrix is inverse Matrix of otherMatrix, false otherwise.
         */
        constexpr bool isInverseMatrixTo(const Matrix& otherMatrix) const noexcept
        {
            OGLS_SQUARE_MATRIX_STATIC_ASSERT(N, M, isInverseMatrixTo);
            return (*this * otherMatrix).isIdentityMatrix();
        }

        /**
         * \brief Checks if the Matrix is a matrix of ones (matrix where every entry is equal to one).
         *
         * \return true if the Matrix is a matrix of ones, false otherwise.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr bool isMatrixOfOnes() const noexcept
        {
            for (const auto element : m_data)
            {
                if (ogls::helpers::isFloatsNotEqual(element, 1.0f))
                {
                    return false;
                }
            }

            return true;
        }

        /**
         * \brief Checks if the Matrix is a zero-matrix (all elements are 0.0).
         *
         * \return true if the Matrix is a zero-matrix, false otherwise.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr bool isZeroMatrix() const noexcept
        {
            for (const auto element : m_data)
            {
                if (ogls::helpers::isFloatsNotEqual(element, 0.0f))
                {
                    return false;
                }
            }

            return true;
        }

        /**
         * \brief Executes the provided functor on every element of the Matrix.
         *
         * Throws, if the functor throws.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of BaseMatrix::Index structure and the value of
         * the current element via second argument. Functor must return new value of the passed element.
         */
        template<MatrixFunctor Functor, typename = IsNotNullMatrix<N, M>>
        constexpr void performOnEvery(Functor functor)
        {
            for (auto i = size_t{0}; i < m_rowsNumber; ++i)
            {
                for (auto j = size_t{0}; j < m_columnsNumber; ++j)
                {
                    const auto index = i * m_columnsNumber + j;
                    m_data[index]    = functor(Index{.rows = i, .columns = j}, m_data[index]);
                }
            }
        }

        /**
         * \brief Executes the provided functor on every element of the Matrix without changing the element of Matrix.
         *
         * This method allows to iterate over elements of the Matrix and perform an action,
         * which doesn't change the elements of the Matrix.
         *
         * Throws, if the functor throws.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of BaseMatrix::Index structure and the value of
         * the current element via second argument.
         */
        template<MatrixConstFunctor Functor, typename = IsNotNullMatrix<N, M>>
        constexpr void performOnEvery(Functor functor) const
        {
            for (auto i = size_t{0}; i < m_rowsNumber; ++i)
            {
                for (auto j = size_t{0}; j < m_columnsNumber; ++j)
                {
                    functor(Index{.rows = i, .columns = j}, m_data[i * m_columnsNumber + j]);
                }
            }
        }

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
        template<typename = IsNotNullMatrix<N, M>>
        constexpr void setValue(size_t row, size_t column, float value)
        {
            if (row >= m_rowsNumber || column >= m_columnsNumber)
            {
                throw std::out_of_range{
                  formatInvalidElementPositionErrorMessage(m_rowsNumber, m_columnsNumber,
                                                           Index{.rows = row, .columns = column})};
            }
            m_data[row * m_columnsNumber + column] = value;
        }

        /**
         * \brief Sets the value at the specified position in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param elementPosition - position of the element in the Matrix.
         * \param value           - the value to set.
         * \throw std::out_of_range.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr void setValue(const Index& elementPosition, float value)
        {
            setValue(elementPosition.rows, elementPosition.columns, value);
        }

        /**
         * \brief Returns a std::string representation of the Matrix object with all elements and size.
         *
         * \param columnWidth - is a minimum number of characters (digits + dot) of Matrix element to be displayed.
         */
        std::string toFullString(int columnWidth = 6) const
        {
            auto ss = std::stringstream{};

            ss << "Matrix " << m_rowsNumber << "x" << m_columnsNumber;

            if (isNullMatrix())
            {
                return ss.str();
            }
            else
            {
                ss << ":\n";
            }

            auto el = this->begin();
            ss << "  | " << std::left << std::setw(columnWidth) << (*el).getValue()
               << (m_columnsNumber > 1 ? ", " : "");

            for (++el; el != end(); ++el)
            {
                if ((*el).j == 0)
                {
                    ss << " |\n  | ";
                }

                ss << std::left << std::setw(columnWidth) << (*el).getValue();

                if ((*el).j != m_columnsNumber - 1)
                {
                    ss << ", ";
                }
            }

            ss << " |";

            return ss.str();
        }

        /**
         * \brief Returns new Matrix as the result of transpose of this Matrix.
         */
        template<typename = IsNotNullMatrix<N, M>>
        constexpr auto transpose() const noexcept
        {
            auto result = Matrix<M, N>{};

            for (const auto& el : *this)
            {
                result.setValue(el.j, el.i, el.getValue());
            }

            return result;
        }

    private:
        template<typename = IsNotNullMatrix<N, M>>
        static std::string formatInvalidElementPositionErrorMessage(size_t rowsNumber, size_t columnsNumber,
                                                                    const Index& position)
        {
            return std::format("Matrix size is {}x{}, but passed element position is [{}][{}]", rowsNumber,
                               columnsNumber, position.rows, position.columns);
        }

    private:
        /**
         * \brief The elements of Matrix.
         */
        // clang-format off
        std::array<float, N * M> m_data = std::array<float, N * M>{};  // clang-format on

};  // class Matrix

//------ OPERATIONS ON MATRIX

/**
 * \brief Checks equality of two Matrix.
 *
 * \return true if two Matrix have equal elements, false otherwise.
 */
template<size_t N, size_t M>
constexpr bool operator==(const Matrix<N, M>& m1, const Matrix<N, M>& m2) noexcept
{
    if constexpr (isNullMatrixCheck(N, M))
    {
        return true;
    }
    else
    {
        auto result = true;
        m1.performOnEvery(
          [&](BaseMatrix::Index pos, float element)
          {
              if (helpers::isFloatsNotEqual(element, m2.getValue(pos)))
              {
                  result = false;
                  return;
              }
          });

        return result;
    }
}

/**
 * \brief Checks if two Matrix are not equal.
 *
 * \return true if two Matrix have different elements, false otherwise.
 */
template<size_t N, size_t M>
constexpr bool operator!=(const Matrix<N, M>& m1, const Matrix<N, M>& m2) noexcept
{
    return !(m1 == m2);
}

/**
 * \brief Adds two Matrix.
 *
 * \return Matrix, which is the result of addition of two Matrix.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator+(const Matrix<N, M>& m1, const Matrix<N, M>& m2) noexcept
{
    auto result = Matrix{m1};
    // clang-format off
    result.performOnEvery([&](BaseMatrix::Index pos, float element) { return element + m2.getValue(pos); });  // clang-format on
    return result;
}

/**
 * \brief Adds number to a Matrix.
 *
 * \param num - a number to add to every element of the Matrix.
 * \param m   - a source Matrix.
 * \return new Matrix, which is a result of addition of the number to every element of Matrix m.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator+(float num, const Matrix<N, M>& m) noexcept
{
    auto result  = Matrix{m};
    result      += num;
    return result;
}

/**
 * \brief Adds number to a Matrix.
 *
 * \param m   - a source Matrix.
 * \param num - a number to add to every element of the Matrix.
 * \return new Matrix, which is a result of addition of the number to every element of Matrix m.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator+(const Matrix<N, M>& m, float num) noexcept
{
    return num + m;
}

/**
 * \brief Subtracts two Matrix.
 *
 * \return Matrix, which is the result of subtraction of two Matrix.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator-(const Matrix<N, M>& m1, const Matrix<N, M>& m2) noexcept
{
    auto result = Matrix{m1};
    // clang-format off
    result.performOnEvery([&](BaseMatrix::Index pos, float element) { return element - m2.getValue(pos); });  // clang-format on
    return result;
}

/**
 * \brief Subtracts the number from the Matrix.
 *
 * \param m   - a source Matrix.
 * \param num - a number to subtract from every element of the Matrix.
 * \return new Matrix, which is a result of subtraction of the number from every element of Matrix m.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator-(const Matrix<N, M>& m, float num) noexcept
{
    auto result  = Matrix{m};
    result      -= num;
    return result;
}

/**
 * \brief Multiplies two Matrix.
 *
 * \note The number of columns of the Matrix m1 must be equal to the number of rows of the Matrix m2.
 * \return Matrix, which is the result of multiplication of two Matrix.
 */
// clang-format off
template<size_t N1, size_t M1, size_t N2, size_t M2,
         typename = IsNotNullMatrix<N1, M1>,
         typename = IsNotNullMatrix<N2, M2>,
         typename = std::enable_if_t<M1 == N2>>
// clang-format on
constexpr auto operator*(const Matrix<N1, M1>& m1, const Matrix<N2, M2>& m2) noexcept
{
    auto result = Matrix<N1, M2>{};

    for (auto i = size_t{0}; i < N1; ++i)
    {
        for (auto j = size_t{0}; j < M2; ++j)
        {
            auto resultElement = float{0.0};

            for (auto iInner = size_t{0}; iInner < M1; ++iInner)
            {
                resultElement += m1.getValue(i, iInner) * m2.getValue(iInner, j);
            }

            result.setValue(i, j, resultElement);
        }
    }

    return result;
}

/**
 * \brief Multiplies all elements of the Matrix by number.
 *
 * \param num - a number to multiply elements of the Matrix by.
 * \param m   - a source Matrix.
 * \return new Matrix, which is a result of multiplication of every element of Matrix m by number.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator*(float num, const Matrix<N, M>& m) noexcept
{
    auto result  = Matrix{m};
    result      *= num;
    return result;
}

/**
 * \brief Multiplies all elements of the Matrix by number.
 *
 * \param m   - a source Matrix.
 * \param num - a number to multiply elements of the Matrix by.
 * \return new Matrix, which is a result of multiplication of every element of Matrix m by number.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator*(const Matrix<N, M>& m, float num) noexcept
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
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr auto operator/(const Matrix<N, M>& m, float num)
{
    if (num == 0.0f)
    {
        throw ogls::exceptions::DivisionByZeroException{std::format("{} / 0.0", m.toSizeOnlyString())};
    }

    auto result  = Matrix{m};
    result      /= num;
    return result;
}

}  // namespace ogls::mathCore

#undef OGLS_DEFINE_MATRIX_ELEMENT_ACCESSOR
#undef OGLS_SQUARE_MATRIX_STATIC_ASSERT

#endif
