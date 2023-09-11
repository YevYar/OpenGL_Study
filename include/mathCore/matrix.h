#ifndef OGLS_MATHCORE_MATRIX_H
#define OGLS_MATHCORE_MATRIX_H

#include <format>
#include <functional>
#include <string>

#include "helpers/debugHelpers.h"
#include "helpers/macros.h"

namespace ogls::mathCore
{
/**
 * \brief IteratorImpl is a concept, which specifies the class, which can be used as a base class of
 * ogls::mathCore::Matrix::Iterator.
 *
 * Impl is a class, which defines the order, in which the Iterator is moved, as well as mapping between iterator steps
 * and actual elements of the underlying data container of Matrix (std::vector).
 *
 * 1. Impl must be inherited from ogls::mathCore::Matrix::BaseIterator.
 * 2. Impl must have the method distanceTo(const Impl&), which calculates the signed number of elements between this
 * Iterator and Iterator j. As a rule of thumb, it must return the signed number of calls of decrement/increment
 * operator to move this Iterator to the position of the Iterator j. Return value is positive if the decrement
 * operator must be applied to move to the position of Iterator j, otherwise it is negative.
 * The type of the return value must be ogls::mathCore::Matrix::BaseIterator::difference_type.
 * 3. Impl must have the method calculateOffset(Matrix::BaseIterator::difference_type N), which converts the number of
 * iterator steps to the actual number of elements in underlying data container of Matrix (std::vector),
 * that is, how many elements of the vector the iterator moves during N increment/decrement steps.
 * The type of the return value must be Matrix::BaseIterator::difference_type.
 *
 * \param Impl         - a type to check constraints.
 * \param BaseIterator - must be Matrix::BaseIterator.
 */
template<typename Impl, typename BaseIterator>
concept IteratorImpl = requires(Impl obj) {
    requires std::is_base_of_v<BaseIterator, Impl>;
    // clang-format off
    {obj.distanceTo(obj)}    -> std::same_as<std::ptrdiff_t>;
    {obj.calculateOffset(1)} -> std::same_as<std::ptrdiff_t>;  // clang-format on
};

/**
 * \brief MatrixIterator is a concept, which specifies the class, which is some instantiation of
 * ogls::mathCore::Matrix::Iterator.
 *
 * \param DerivedIterator - a type to check constraints.
 * \param BaseIterator    - must be ogls::mathCore::Matrix::BaseIterator.
 */
template<typename DerivedIterator, typename BaseIterator>
concept MatrixIterator = requires(DerivedIterator obj) {
    requires std::is_base_of_v<BaseIterator, DerivedIterator>;
    // clang-format off
    {obj.compareWithUnderlyingIterator(std::declval<std::vector<float>::iterator>())}
      -> std::same_as<decltype(std::declval<std::vector<float>::iterator>()
                               <=> std::declval<std::vector<float>::iterator>())>;
    {obj.equalToUnderlyingIterator(std::declval<std::vector<float>::iterator>())} -> std::same_as<bool>;
    // clang-format on
};

/**
 * \brief Matrix represents a N*M dimensional matrix.
 *
 * Matrix with dimensions equal to 0 can be created only by default constructor.
 * Such Matrix is considered as null Matrix.
 * All other constructors require not zero values of dimensions.
 *
 * Indexes are counted starting from 0.
 *
 * Matrix provides class users with iterators, which can be used with STL algorithms, which don't change
 * the number of elements in provided data container (see Matrix::Iterator).
 *
 * \code{.cpp}
 * // Usage of range for-loop
 * for (const auto& el : someMatrix)
 * {
 *     std::cout << "element (" << el.i << ", " << el.j << ") = " << el.getValue() << "\n";
 *     el.getValue() = el.getValue() + el.i + el.j;
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
 */
class Matrix
{
    public:
        /**
         * \brief Matrix::BaseIterator is a base class of iterator types of the Matrix.
         *
         * It contains necessary fields and is used in Matrix iterator constraints.
         */
        class BaseIterator
        {
            public:
                // The STL conventions
                using difference_type = std::ptrdiff_t;

            protected:
                constexpr BaseIterator() noexcept = default;

                constexpr BaseIterator(size_t columnsNumber, size_t maxIndex, size_t rowsNumber) noexcept :
                    m_columnsNumber{columnsNumber}, m_maxIndex{maxIndex}, m_rowsNumber{rowsNumber}
                {
                }

            protected:
                /**
                 * \brief The number of columns in represented Matrix.
                 */
                const size_t m_columnsNumber = {0};
                /**
                 * \brief The row index of the current element.
                 */
                size_t       m_currentI      = {0};
                /**
                 * \brief The column index of the current element.
                 */
                size_t       m_currentJ      = {0};
                /**
                 * \brief The maximum element index in the underlying Matrix data container.
                 */
                const size_t m_maxIndex      = {0};
                /**
                 * \brief The number of rows in represented Matrix.
                 */
                const size_t m_rowsNumber    = {0};

        };  // class BaseIterator

        /**
         * \brief Matrix::DiagonalIteratorImpl provides an implementation of necessary methods for Iterator
         * to iterate over Matrix elements, which belong to the main diagonal of the Matrix ((0, 0), (1, 1) etc.).
         *
         * It must be used only with square Matrix. Otherwise it usage can lead to runtime errors.
         *
         * \see Iterator.
         */
        class DiagonalIteratorImpl : public BaseIterator
        {
            public:
                using typename BaseIterator::difference_type;

            public:
                /**
                 * \see ogls::mathCore::IteratorImpl.
                 */
                constexpr difference_type calculateOffset(difference_type n) const noexcept;

                /**
                 * \see ogls::mathCore::IteratorImpl.
                 */
                constexpr difference_type distanceTo(const DiagonalIteratorImpl& j) const noexcept
                {
                    return m_currentI - j.m_currentI;
                }

            protected:
                using BaseIterator::BaseIterator;

        };  // class DiagonalIteratorImpl

        /**
         * \brief Matrix::RowMajorIteratorImpl provides an implementation of necessary methods for Iterator
         * to iterate over Matrix elements in row-major order from element (0, 0) to element (N-1, M-1).
         *
         * \see Iterator.
         */
        class RowMajorIteratorImpl : public BaseIterator
        {
            public:
                using typename BaseIterator::difference_type;

            public:
                /**
                 * \see ogls::mathCore::IteratorImpl.
                 */
                constexpr difference_type calculateOffset(difference_type n) const noexcept
                {
                    return n;
                }

                /**
                 * \see ogls::mathCore::IteratorImpl.
                 */
                constexpr difference_type distanceTo(const RowMajorIteratorImpl& j) const noexcept
                {
                    const auto thisPosition = size_t{m_currentI * m_columnsNumber + m_currentJ};
                    const auto jPosition    = size_t{j.m_currentI * j.m_columnsNumber + j.m_currentJ};
                    return thisPosition - jPosition;
                }

            protected:
                using BaseIterator::BaseIterator;

        };  // class RowMajorIteratorImpl

        /**
         * \brief Matrix::Iterator provides an access to Matrix elements in implementation defined order (see
         * ConcreteIteratorImpl).
         *
         * Iterator implements std::random_access_iterator interface. Dereference operator of the Iterator returns
         * an Element object, which provides an access to the pointed element of the Matrix and information about the
         * index of this element.
         *
         * Iterator follows the convention of the STL iterators to not throw an exception, when its user tries to
         * dereference invalid Iterator (see isValid()). However move operations as well as copy assignment
         * throw an exception to prevent changing the number of elements of Matrix data container outside the logic
         * of Matrix class. Such algorithms as std::remove() cannot be used with Iterator.
         *
         * \param ElementType            - the type of referenced elements (float, const float).
         * \param UnderlyingIteratorType - the type of the iterator, which is used by the data container of Matrix
         * (std::vector<float>::iterator, std::vector<float>::const_iterator).
         * \param ConcreteIteratorImpl   - the base class, which provides an implementation of necessary stuff
         * (see ogls::mathCore::IteratorImpl).
         * \see isValid().
         */
        template<typename ElementType, std::random_access_iterator UnderlyingIteratorType,
                 IteratorImpl<Matrix::BaseIterator> ConcreteIteratorImpl>
        class Iterator final : protected ConcreteIteratorImpl
        {
            public:
                /**
                 * \brief Element keeps the data of the Matrix element, on which the Iterator points.
                 *
                 * Element provides necessary data about the Matrix element -
                 * its row and column indexes as well as the element data.
                 */
                struct Element
                {
                    public:
                        /**
                         * \brief Allows copy assignment of the float value.
                         *
                         * Changes the referenced Matrix element. Makes it possible to use algorithms like std::fill().
                         */
                        template<typename = std::enable_if_t<!std::is_const_v<ElementType>>>
                        constexpr Element& operator=(ElementType newValue) noexcept
                        {
                            getValue() = newValue;
                            return *this;
                        }

                        /**
                         * \brief Returns an ElementType representation of the Element object.
                         *
                         * Makes it possible to use algorithms like std::accumulate().
                         *
                         * \see getValue().
                         */
                        constexpr operator ElementType() const noexcept
                        {
                            return getValue();
                        }

                        /**
                         * \brief Returns a std::string representation of the Element object.
                         */
                        constexpr operator std::string() const
                        {
                            return std::format("Matrix::Iterator::Element({}, {}) = {}", i, j, getValue());
                        }

                        /**
                         * \brief Returns the reference on the referenced Matrix element.
                         *
                         * Use it to change the referenced Matrix element: getValue() = newValue;
                         */
                        constexpr ElementType& getValue() const noexcept
                        {
                            OGLS_ASSERT(value != nullptr);
                            return *value;
                        }

                    public:
                        /**
                         * \brief The index of the Matrix row of this Element (counts from 0).
                         *
                         * Changing this field has no effect on Iterator state.
                         */
                        size_t       i     = {0};
                        /**
                         * \brief The index of the Matrix column of this Element (counts from 0).
                         *
                         * Changing this field has no effect on Iterator state.
                         */
                        size_t       j     = {0};
                        /**
                         * \brief The pointer to the value of the Element.
                         */
                        ElementType* value = nullptr;

                };  // struct Element

                // The following type aliases are STL conventions
                using typename ConcreteIteratorImpl::difference_type;
                using iterator_category = std::random_access_iterator_tag;
                using iterator_concept  = std::random_access_iterator_tag;
                using value_type        = Element;
                using reference         = value_type;

            public:
                /**
                 * \brief Constructs the default Iterator.
                 *
                 * The created instance has an null-initialized state, which is the kind of invalid state (see
                 * isValid()). Don't dereference it!
                 *
                 * \see isValid().
                 */
                constexpr Iterator() noexcept                = default;  // it's required by iterator concept
                /**
                 * \brief Constructs new Iterator as copy of the other Iterator.
                 *
                 * The copy constructor doesn't throw an exception,
                 * because the state of the source Iterator isn't affected by copy operation.
                 */
                constexpr Iterator(const Iterator&) noexcept = default;  // it's required by iterator concept
                /**
                 * \brief Constructs new Iterator as move-copy of the other Iterator.
                 *
                 * The move-copy constructor throws an exception,
                 * because the state of the source Iterator is affected by move operation.
                 *
                 * \throw std::domain_error.
                 */
                constexpr Iterator(Iterator&&);  // it's required by iterator concept

                /**
                 * \brief Copies the state of the other Iterator.
                 *
                 * The operation throws an exception,
                 * because the state of this Iterator is erased and replaced by the state of other Iterator.
                 *
                 * \throw std::domain_error.
                 */
                constexpr Iterator& operator=(const Iterator&);  // it's required by iterator concept
                /**
                 * \brief Move-copies the state of the other Iterator.
                 *
                 * The operation throws an exception,
                 * because the state of this Iterator is erased and replaced by the state of other Iterator.
                 *
                 * \throw std::domain_error.
                 */
                constexpr Iterator& operator=(Iterator&&);  // it's required by iterator concept

                // concept forward_iterator
                constexpr reference operator*() const noexcept;
                constexpr Iterator& operator++() noexcept;  // prefix ++
                constexpr Iterator  operator++(int) noexcept;  // postfix ++

                /**
                 * \brief Checks equality of the object of this Iterator type and any other Iterator,
                 * which is derived from BaseIterator.
                 */
                template<MatrixIterator<BaseIterator> OtherIterator>
                friend constexpr bool operator==(const Iterator& i, const OtherIterator& j) noexcept
                {
                    // This trick is used, because this operator== cannot be a friend to both Iterator and OtherIterator
                    // at the same time. So the public method of OtherIterator is called to compare m_dataIterator of
                    // object j with m_dataIterator of object i, for which operator== is a friend and has an access to
                    // m_dataIterator.
                    return j.equalToUnderlyingIterator(i.m_dataIterator);
                }

                /**
                 * \brief Checks non-equality of the object of this Iterator type and any other Iterator,
                 * which is derived from BaseIterator.
                 */
                template<MatrixIterator<BaseIterator> OtherIterator>
                friend constexpr bool operator!=(const Iterator& i, const OtherIterator& j) noexcept
                {
                    return !(i == j);
                }

                // concept bidirectional_iterator
                constexpr Iterator& operator--() noexcept;  // prefix --
                constexpr Iterator  operator--(int) noexcept;  // postfix --

                // concept random_access_iterator
                constexpr Iterator& operator+=(difference_type n) noexcept;
                constexpr Iterator& operator-=(difference_type n) noexcept;
                /**
                 * \brief Provides access to element at a certain index relative to the current iterator position.
                 *
                 * It's used to mimic the behavior of array subscripting for your iterator type.
                 *
                 * \param index - an offset from the current Iterator position to another element.
                 */
                constexpr reference operator[](difference_type index) const noexcept;

                friend constexpr Iterator operator+(const Iterator& i, difference_type n) noexcept
                {
                    auto temp  = Iterator{i};
                    temp      += n;
                    return temp;
                }

                friend constexpr Iterator operator+(difference_type n, const Iterator& i) noexcept
                {
                    return i + n;
                }

                friend constexpr Iterator operator-(const Iterator& i, difference_type n) noexcept
                {
                    auto temp  = Iterator{i};
                    temp      -= n;
                    return temp;
                }

                friend constexpr difference_type operator-(const Iterator& i, const Iterator& j) noexcept
                {
                    return i.distanceTo(j);
                }

                /**
                 * \brief Compares the object of this Iterator type with any other Iterator,
                 * which is derived from BaseIterator.
                 */
                template<MatrixIterator<BaseIterator> OtherIterator>
                friend constexpr auto operator<=>(const Iterator& i, const OtherIterator& j) noexcept
                {
                    // This trick is used, because this operator<=> cannot be a friend to both Iterator and
                    // OtherIterator at the same time. So the public method of OtherIterator is called to compare
                    // m_dataIterator of object j with m_dataIterator of object i, for which operator<=> is a friend and
                    // has an access to m_dataIterator.
                    return j.compareWithUnderlyingIterator(i.m_dataIterator);
                }

                /**
                 * \brief Compares underlying iterator of this object with underlying iterator of another object.
                 */
                template<std::random_access_iterator UnderlyingIterator>
                constexpr auto compareWithUnderlyingIterator(const UnderlyingIterator& i) const noexcept
                {
                    return m_dataIterator <=> i;
                }

                /**
                 * \brief Checks equality of underlying iterator of this object with underlying iterator of another
                 * object.
                 */
                template<std::random_access_iterator UnderlyingIterator>
                constexpr bool equalToUnderlyingIterator(const UnderlyingIterator& i) const noexcept
                {
                    return m_dataIterator == i;
                }

                /**
                 * \brief Checks if Iterator is in valid state.
                 *
                 * An invalid state of the Iterator means that the Iterator point on the next element after
                 * the end of the data container of the parent Matrix (element with I (element row index) = N
                 * (a number of rows in the Matrix) and J (element column index) = 0).
                 *
                 * \return true if Iterator is in valid state, false otherwise.
                 */
                constexpr bool isValid() const noexcept;

            private:
                /**
                 * \brief Constructs the Iterator over provided data iterator.
                 *
                 * \param rowsNumber    - a number of rows in the parent Matrix.
                 * \param columnsNumber - a number of columns in the parent Matrix.
                 * \param dataIterator  - an iterator for the data container of the parent Matrix.
                 * \param isEndIterator - indicator, which is necessary for correct initialisation of the Iterator.
                 * If true the Iterator is initialised to point on the next element after the end of the data container
                 * of the parent Matrix (element with I (element row index) = N (a number of rows in the Matrix) and
                 * J (element column index) = 0). If false the Iterator points on the element (0, 0).
                 * \see isValid().
                 */
                constexpr Iterator(size_t rowsNumber, size_t columnsNumber, UnderlyingIteratorType dataIterator,
                                   bool isEndIterator = false) noexcept;

                /**
                 * \brief Sets the invalid state of the Iterator.
                 *
                 * \see isValid().
                 */
                constexpr void setInvalidState() noexcept;
                /**
                 * \brief Updates the position (row and column indexes) on which Iterator points.
                 *
                 * \param i - the index of the Matrix row of the current Element.
                 * \param j - the index of the Matrix column of the current Element.
                 */
                constexpr void updateCurrentPosition(size_t i, size_t j) noexcept;

                /**
                 * \brief Moves Iterator on n positions.
                 *
                 * It must be equal to n calls of increment operator if n is positive and
                 * to n calls of decrement operator if n is negative.
                 *
                 * \param n - an offset from the current Iterator position to another element.
                 */
                constexpr void moveIterator(difference_type n) noexcept;

            private:
                /**
                 * \brief The underlying iterator on the Matrix data.
                 */
                UnderlyingIteratorType m_dataIterator;
                bool                   m_isDefaultInitialised = true;


                friend class Matrix;

        };  // class Iterator

        /**
         * \brief Matrix::Size represents a dimension of the Matrix or specific index in the Matrix.
         */
        struct Size
        {
            public:
                /**
                 * \brief Returns a std::string representation of the Size object.
                 */
                operator std::string() const
                {
                    return std::format("Matrix::Size(rows={}, columns={})", rows, columns);
                }

            public:
                size_t rows    = {0};
                size_t columns = {0};


                friend bool operator==(const Size&, const Size&) = default;

        };  // struct Size

        /**
         * \brief Matrix::Index represents an index (position) of the element in the Matrix.
         */
        using Index = Size;

        // The following type aliases are the conventions of STL

        /**
         * \brief Iterates over Matrix elements in row-major order from element (0, 0) to element (N-1, M-1)
         * without providing ability to change the referenced Matrix element.
         */
        using const_iterator = Iterator<const float, std::vector<float>::const_iterator, RowMajorIteratorImpl>;
        /**
         * \brief Iterates over Matrix elements in row-major order from element (0, 0) to element (N-1, M-1).
         */
        using iterator       = Iterator<float, std::vector<float>::iterator, RowMajorIteratorImpl>;

        // Non-conventional iterator aliases

        /**
         * \brief Iterates over Matrix elements, which belong to the main diagonal of the Matrix ((0, 0), (1, 1) etc.)
         * without providing ability to change the referenced Matrix element.
         */
        using const_diagonal_iterator = Iterator<const float, std::vector<float>::const_iterator, DiagonalIteratorImpl>;
        /**
         * \brief Iterates over Matrix elements, which belong to the main diagonal of the Matrix ((0, 0), (1, 1) etc.).
         */
        using diagonal_iterator       = Iterator<float, std::vector<float>::iterator, DiagonalIteratorImpl>;

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
         * Both dimensions must be more then 0 and all columns must have the same size,
         * otherwise the exception is thrown.
         *
         * \param values - the elements of the Matrix.
         * \throw ogls::exceptions::MatrixException().
         */
        explicit Matrix(std::initializer_list<std::initializer_list<float>> values);
        /**
         * \brief Constructs a Matrix with the specified elements.
         *
         * It is assumed that the values are provided in row-major order. If the input list has more elements then
         * the rowsNumber * columnsNumber, the values out of this range are ignored.
         *
         * The exception is thrown if one of dimensions is 0 or if the size of the input list is less then
         * the expected size based on the provided dimensions.
         *
         * \param rowsNumber    - a number of rows in the Matrix.
         * \param columnsNumber - a number of columns in the Matrix.
         * \param values        - the elements of the Matrix.
         * \throw ogls::exceptions::MatrixException().
         */
        Matrix(size_t rowsNumber, size_t columnsNumber, std::initializer_list<float> values);
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

        //------ RANGE STUFF

        /**
         * \brief Returns an Iterator to the beginning.
         */
        iterator                begin() noexcept;
        /**
         * \brief Returns const Iterator to the beginning.
         */
        const_iterator          begin() const noexcept;
        /**
         * \brief Returns a DiagonalIterator to the beginning.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        diagonal_iterator       beginDiagonal();
        /**
         * \brief Returns const DiagonalIterator to the beginning.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        const_diagonal_iterator beginDiagonal() const;
        /**
         * \brief Returns const Iterator to the beginning.
         */
        const_iterator          cbegin() const noexcept;
        /**
         * \brief Returns const DiagonalIterator to the beginning.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        const_diagonal_iterator cbeginDiagonal() const;
        /**
         * \brief Returns an Iterator to the end.
         */
        iterator                end() noexcept;
        /**
         * \brief Returns const Iterator to the end.
         */
        const_iterator          end() const noexcept;
        /**
         * \brief Returns a DiagonalIterator to the end.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        diagonal_iterator       endDiagonal();
        /**
         * \brief Returns const DiagonalIterator to the end.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        const_diagonal_iterator endDiagonal() const;
        /**
         * \brief Returns const Iterator to the end.
         */
        const_iterator          cend() const noexcept;
        /**
         * \brief Returns const DiagonalIterator to the end.
         *
         * Throws std::domain_error if the Matrix isn't square Matrix.
         *
         * \throw std::domain_error.
         */
        const_diagonal_iterator cendDiagonal() const;

        /**
         * \brief Returns the number of elements in the Matrix.
         */
        size_t size() const noexcept
        {
            return m_rowsNumber * m_columnsNumber;
        }

        //------

        /**
         * \brief Returns the dimensionality of the Matrix.
         */
        Size dimensionality() const noexcept
        {
            return Size{.rows = m_rowsNumber, .columns = m_columnsNumber};
        }

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
        float getValue(const Index& elementPosition) const;
        /**
         * \brief Checks if the Matrix is an identity matrix (square matrix with ones on the main diagonal
         * and zeros elsewhere).
         *
         * \return true if the Matrix is an identity matrix, false otherwise.
         */
        bool  isIdentityMatrix() const noexcept;
        /**
         * \brief Checks if the Matrix is a matrix of ones (matrix where every entry is equal to one).
         *
         * \return true if the Matrix is a matrix of ones, false otherwise.
         */
        bool  isMatrixOfOnes() const noexcept;

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
         * \brief Checks if the Matrix is a square Matrix (dimensions are equal).
         *
         * \return true if the Matrix is a square Matrix, false otherwise.
         */
        bool isSquareMatrix() const noexcept
        {
            return m_rowsNumber != 0 && m_rowsNumber == m_columnsNumber;
        }

        /**
         * \brief Checks if the Matrix is a zero-matrix (all elements are 0.0).
         *
         * \return true if the Matrix is a zero-matrix, false otherwise.
         */
        bool        isZeroMatrix() const noexcept;
        /**
         * \brief Executes the provided functor on every element of the Matrix.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of Matrix::Index structure and the value of the current
         * element via second argument. Functor must return new value of the passed element.
         */
        void        performOnEvery(std::function<float(Index, float)> functor);
        /**
         * \brief Executes the provided functor on every element of the Matrix without changing the element of Matrix.
         *
         * This method allows to iterate over elements of the Matrix and perform an action,
         * which doesn't change the elements of the Matrix.
         *
         * \param functor - a function to execute on every element of the Matrix. Such functor receives the position
         * of the current element via first argument as object of Matrix::Index structure and the value of the current
         * element via second argument.
         */
        void        performOnEvery(std::function<void(Index, float)> functor) const;
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
        void        setValue(size_t row, size_t column, float value);
        /**
         * \brief Sets the value at the specified position in the Matrix.
         *
         * Indexes are counted starting from 0.
         *
         * \param elementPosition - position of the element in the Matrix.
         * \param value           - the value to set.
         * \throw std::out_of_range.
         */
        void        setValue(const Index& elementPosition, float value);
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
