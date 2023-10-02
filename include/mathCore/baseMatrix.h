#ifndef OGLS_MATHCORE_BASE_MATRIX_H
#define OGLS_MATHCORE_BASE_MATRIX_H

#include <format>
#include <string>

#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/macros.h"

namespace ogls::mathCore
{
/**
 * \brief IteratorImpl is a concept, which specifies the class, which can be used as a base class of
 * ogls::mathCore::BaseMatrix::Iterator.
 *
 * Impl is a class, which defines the order, in which the Iterator is moved, as well as mapping between iterator steps
 * and actual elements of the underlying data container of Matrix (std::array).
 *
 * 1. Impl must be inherited from ogls::mathCore::BaseMatrix::BaseIterator.
 * 2. Impl must have the method distanceTo(const Impl&), which calculates the signed number of elements between this
 * Iterator and Iterator j. As a rule of thumb, it must return the signed number of calls of decrement/increment
 * operator to move this Iterator to the position of the Iterator j. Return value is positive if the decrement
 * operator must be applied to move to the position of Iterator j, otherwise it is negative.
 * The type of the return value must be ogls::mathCore::BaseMatrix::BaseIterator::difference_type.
 * 3. Impl must have the method calculateOffset(BaseMatrix::BaseIterator::difference_type N), which converts the number
 * of iterator steps to the actual number of elements in underlying data container of Matrix (std::array),
 * that is, how many elements of the array the iterator moves during N increment/decrement steps.
 * The type of the return value must be BaseMatrix::BaseIterator::difference_type.
 *
 * \param Impl         - a type to check constraints.
 * \param BaseIterator - must be BaseMatrix::BaseIterator.
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
 * ogls::mathCore::BaseMatrix::Iterator.
 *
 * \param DerivedIterator        - a type to check constraints.
 * \param BaseIterator           - must be ogls::mathCore::BaseMatrix::BaseIterator.
 * \param UnderlyingIteratorType - the type of underlaying iterator (std::array::iterator or std::array::const_iterator).
 */
template<typename DerivedIterator, typename BaseIterator, typename UnderlyingIteratorType>
concept MatrixIterator = requires(DerivedIterator obj) {
    requires std::is_base_of_v<BaseIterator, DerivedIterator>;
    // clang-format off
    {obj.compareWithUnderlyingIterator(std::declval<UnderlyingIteratorType>())}
      -> std::same_as<decltype(std::declval<UnderlyingIteratorType>()
                               <=> std::declval<UnderlyingIteratorType>())>;
    {obj.equalToUnderlyingIterator(std::declval<UnderlyingIteratorType>())} -> std::same_as<bool>;
    // clang-format on
};

/**
 * \brief BaseMatrix contains dimensionality of the Matrix and provides methods to get information about this.
 */
class BaseMatrix
{
    public:
        /**
         * \brief BaseMatrix::BaseIterator is a base class of iterator types of the Matrix.
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

                /**
                 * \brief Constructs a BaseIterator with the specified number of rows and columns.
                 *
                 * \param rowsNumber    - a number of rows in the Matrix.
                 * \param columnsNumber - a number of columns in the Matrix.
                 * \param maxIndex      - the maximum element index in the underlying Matrix data container.
                 */
                constexpr BaseIterator(size_t rowsNumber, size_t columnsNumber, size_t maxIndex) noexcept :
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
         * \brief BaseMatrix::DiagonalIteratorImpl provides an implementation of necessary methods for Iterator
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
                constexpr difference_type calculateOffset(difference_type iteratorN) const noexcept
                {
                    if ((m_currentI == m_rowsNumber - 1) && (iteratorN > 0))
                    {
                        return 1;
                    }
                    else if ((m_currentI == m_rowsNumber) && (iteratorN < 0))
                    {
                        const auto n = iteratorN + 1;
                        return -1 + (n * static_cast<difference_type>(m_columnsNumber) + n);
                    }
                    else
                    {
                        return iteratorN * static_cast<difference_type>(m_columnsNumber) + iteratorN;
                    }
                }

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
         * \brief BaseMatrix::RowMajorIteratorImpl provides an implementation of necessary methods for Iterator
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
         * \brief BaseMatrix::Iterator provides an access to Matrix elements in implementation defined order (see
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
         * (std::array<float>::iterator, std::array<float>::const_iterator).
         * \param ConcreteIteratorImpl   - the base class, which provides an implementation of necessary stuff
         * (see ogls::mathCore::IteratorImpl).
         * \see isValid().
         */
        template<typename ElementType, std::random_access_iterator UnderlyingIteratorType,
                 IteratorImpl<BaseIterator> ConcreteIteratorImpl>
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
                            setValue(newValue);
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
                        operator std::string() const
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

                        /**
                         * \brief Sets the new value of the referenced element.
                         *
                         * Changes the referenced Matrix element.
                         */
                        template<typename = std::enable_if_t<!std::is_const_v<ElementType>>>
                        constexpr void setValue(ElementType newValue) noexcept
                        {
                            getValue() = newValue;
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
                                   bool isEndIterator = false) noexcept :
                    ConcreteIteratorImpl{rowsNumber, columnsNumber, rowsNumber * columnsNumber - 1},
                    m_dataIterator{dataIterator}, m_isDefaultInitialised{false}
                {
                    if (isEndIterator)
                    {
                        setInvalidState();
                    }
                    else
                    {
                        updateCurrentPosition(0, 0);
                    }
                }

                /**
                 * \brief Constructs new Iterator as move-copy of the other Iterator.
                 *
                 * The move-copy constructor throws an exception,
                 * because the state of the source Iterator is affected by move operation.
                 *
                 * \throw ogls::exceptions::MatrixException().
                 */
                constexpr Iterator(Iterator&&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("Iterator(Iterator&&)");
                }

                /**
                 * \brief Copies the state of the other Iterator.
                 *
                 * The operation throws an exception,
                 * because the state of this Iterator is erased and replaced by the state of other Iterator.
                 *
                 * \throw ogls::exceptions::MatrixException().
                 */
                constexpr Iterator& operator=(const Iterator&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("operator=(const Iterator&)");
                    return *this;
                }

                /**
                 * \brief Move-copies the state of the other Iterator.
                 *
                 * The operation throws an exception,
                 * because the state of this Iterator is erased and replaced by the state of other Iterator.
                 *
                 * \throw ogls::exceptions::MatrixException().
                 */
                constexpr Iterator& operator=(Iterator&&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("operator=(Iterator&&)");
                    return *this;
                }

                // concept forward_iterator
                constexpr reference operator*() const noexcept
                {
                    return isValid()
                             ? Element{.i = this->m_currentI, .j = this->m_currentJ, .value = &(*m_dataIterator)}
                             : Element{.i = this->m_rowsNumber, .j = 0, .value = nullptr};
                }

                constexpr Iterator& operator++() noexcept  // prefix ++
                {
                    moveIterator(1);
                    return *this;
                }

                constexpr Iterator operator++(int) noexcept  // postfix ++
                {
                    auto temp = Iterator{*this};
                    ++*this;
                    return temp;
                }

                /**
                 * \brief Checks equality of the object of this Iterator type and any other Iterator,
                 * which is derived from BaseIterator.
                 */
                template<MatrixIterator<BaseIterator, UnderlyingIteratorType> OtherIterator>
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
                template<MatrixIterator<BaseIterator, UnderlyingIteratorType> OtherIterator>
                friend constexpr bool operator!=(const Iterator& i, const OtherIterator& j) noexcept
                {
                    return !(i == j);
                }

                // concept bidirectional_iterator
                constexpr Iterator& operator--() noexcept  // prefix --
                {
                    moveIterator(-1);
                    return *this;
                }

                constexpr Iterator operator--(int) noexcept  // postfix --
                {
                    auto temp = Iterator{*this};
                    --*this;
                    return temp;
                }

                // concept random_access_iterator
                constexpr Iterator& operator+=(difference_type n) noexcept
                {
                    moveIterator(n);
                    return *this;
                }

                constexpr Iterator& operator-=(difference_type n) noexcept
                {
                    moveIterator(-n);
                    return *this;
                }

                /**
                 * \brief Provides access to element at a certain index relative to the current iterator position.
                 *
                 * It's used to mimic the behavior of array subscripting for your iterator type.
                 *
                 * \param index - an offset from the current Iterator position to another element.
                 */
                constexpr reference operator[](difference_type index) const noexcept
                {
                    return *(*this + index);
                }

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
                template<MatrixIterator<BaseIterator, UnderlyingIteratorType> OtherIterator>
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
                    return i <=> m_dataIterator;
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
                constexpr bool isValid() const noexcept
                {
                    return !m_isDefaultInitialised && this->m_currentI < this->m_rowsNumber
                           && this->m_currentJ < this->m_columnsNumber;
                }

            private:
                /**
                 * \brief Sets the invalid state of the Iterator.
                 *
                 * \see isValid().
                 */
                constexpr void setInvalidState() noexcept
                {
                    updateCurrentPosition(this->m_rowsNumber, 0);
                }

                /**
                 * \brief Updates the position (row and column indexes) on which Iterator points.
                 *
                 * \param i - the index of the Matrix row of the current Element.
                 * \param j - the index of the Matrix column of the current Element.
                 */
                constexpr void updateCurrentPosition(size_t i, size_t j) noexcept
                {
                    this->m_currentI = i;
                    this->m_currentJ = j;
                }

                /**
                 * \brief Moves Iterator on n positions.
                 *
                 * It must be equal to n calls of increment operator if n is positive and
                 * to n calls of decrement operator if n is negative.
                 *
                 * \param n - an offset from the current Iterator position to another element.
                 */
                constexpr void moveIterator(difference_type n) noexcept
                {
                    const auto offset  = this->calculateOffset(n);
                    m_dataIterator    += offset;

                    const auto currentIndex = size_t{this->m_currentI * this->m_columnsNumber + this->m_currentJ};
                    const auto newIndex     = size_t{currentIndex + offset};

                    if (newIndex <= this->m_maxIndex)
                    {
                        this->m_currentJ = newIndex % this->m_columnsNumber;
                        this->m_currentI = (newIndex - this->m_currentJ) / this->m_columnsNumber;
                    }
                    else
                    {
                        setInvalidState();
                    }
                }

                void throwUnexpectedUsageExceptionWithHint(const char* operatorName) const
                {
                    throw ogls::exceptions::MatrixException{
                      std::format("Matrix::Iterator::{} is not allowed by the design of the Matrix class. Hint: "
                                  "attempting to use Matrix::Iterator to modify the underlying Matrix data container "
                                  "outside of the Matrix implementation by adding or removing container elements.",
                                  operatorName)};
                }

            private:
                /**
                 * \brief The underlying iterator on the Matrix data.
                 */
                UnderlyingIteratorType m_dataIterator;
                bool                   m_isDefaultInitialised = true;

        };  // class Iterator

        /**
         * \brief BaseMatrix::Size represents a dimension of the Matrix or specific index in the Matrix.
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
         * \brief BaseMatrix::Index represents an index (position) of the element in the Matrix.
         */
        using Index = Size;

    public:
        /**
         * \brief Retrieves the number of columns in the Matrix.
         *
         * \return the number of columns.
         */
        constexpr size_t getColumnCount() const noexcept
        {
            return m_columnsNumber;
        }

        /**
         * \brief Returns the dimensionality of the Matrix.
         */
        constexpr Size getDimensionality() const noexcept
        {
            return Size{.rows = m_rowsNumber, .columns = m_columnsNumber};
        }

        /**
         * \brief Retrieves the number of rows in the Matrix.
         *
         * \return the number of rows.
         */
        constexpr size_t getRowCount() const noexcept
        {
            return m_rowsNumber;
        }

        /**
         * \brief Checks if the Matrix is a null Matrix (dimensions are 0).
         *
         * \return true if the Matrix is a null Matrix, false otherwise.
         */
        constexpr bool isNullMatrix() const noexcept
        {
            return m_rowsNumber == 0 && m_columnsNumber == 0;
        }

        /**
         * \brief Checks if the Matrix is a square Matrix (dimensions are equal).
         *
         * \return true if the Matrix is a square Matrix, false otherwise.
         */
        constexpr bool isSquareMatrix() const noexcept
        {
            return m_rowsNumber != 0 && m_rowsNumber == m_columnsNumber;
        }

        /**
         * \brief Returns the number of elements in the Matrix.
         */
        constexpr size_t size() const noexcept
        {
            return m_rowsNumber * m_columnsNumber;
        }

        /**
         * \brief Returns a std::string representation of the Matrix object only with size of the Matrix.
         */
        std::string toSizeOnlyString() const
        {
            return std::format("Matrix {}x{}", m_rowsNumber, m_columnsNumber);
        }

    protected:
        constexpr BaseMatrix() noexcept = default;
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(BaseMatrix)

        /**
         * \brief Constructs a Matrix with the specified number of rows and columns.
         *
         * \param rowsNumber    - a number of rows in the Matrix.
         * \param columnsNumber - a number of columns in the Matrix.
         */
        constexpr BaseMatrix(size_t rowsNumber, size_t columnsNumber) noexcept :
            m_columnsNumber{columnsNumber}, m_rowsNumber{rowsNumber}
        {
        }

    protected:
        /**
         * \brief The number of columns.
         */
        size_t m_columnsNumber = {0};
        /**
         * \brief The number of rows.
         */
        size_t m_rowsNumber    = {0};

};  // class BaseMatrix

}  // namespace ogls::mathCore

#endif
