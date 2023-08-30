#include "mathCore/matrix.h"

#include <algorithm>
#include <iomanip>
#include <ranges>
#include <sstream>

#include "exceptions.h"
#include "floats.h"

namespace ogls::mathCore
{
namespace
{
    template<typename Type>
    void initMatrixFromContainer(size_t rowsNumber, size_t columnsNumber, std::vector<float>& matrixData,
                                 Type& container);
    template<typename Type>
    void initMatrixFromContainerOfContainers(size_t rowsNumber, size_t columnsNumber, std::vector<float>& matrixData,
                                             Type& container);
    std::string formatInvalidElementPositionErrorMessage(size_t rowsNumber, size_t columnsNumber,
                                                         const Matrix::Index& position);
    std::string formatZeroDimensionErrorMessage(size_t rowsNumber, size_t columnsNumber);
    void        throwUnexpectedUsageExceptionWithHint(const char* operatorName);

}  // namespace

//------ MATRIX::ITERATOR

static_assert(std::random_access_iterator<Matrix::iterator>);
static_assert(std::ranges::random_access_range<Matrix>);
static_assert(std::ranges::sized_range<Matrix>);
static_assert(std::ranges::common_range<Matrix>);

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr Matrix::Iterator<ElementType, UnderlyingIteratorType>::Iterator(size_t rowsNumber, size_t columnsNumber,
                                                                          UnderlyingIteratorType dataIterator,
                                                                          bool                   isEnd) noexcept :
    m_columnsNumber{columnsNumber},
    m_dataIterator{dataIterator}, m_maxIndex{rowsNumber * columnsNumber - 1}, m_rowsNumber{rowsNumber}
{
    if (isEnd)
    {
        setInvalidState();
    }
    else
    {
        updateCurrentPosition(0, 0);
    }
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr Matrix::Iterator<ElementType, UnderlyingIteratorType>::Iterator(Iterator&&)
{
    throwUnexpectedUsageExceptionWithHint("Iterator(Iterator&&)");
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator=(const Iterator&) -> Iterator&
{
    throwUnexpectedUsageExceptionWithHint("operator=(const Iterator&)");
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator=(Iterator&&) -> Iterator&
{
    throwUnexpectedUsageExceptionWithHint("operator=(Iterator&&)");
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator*() const noexcept -> reference
{
    return isValid() ? Element{.i = m_currentI, .j = m_currentJ, .value = &(*m_dataIterator)}
                     : Element{.i = m_rowsNumber, .j = 0, .value = nullptr};
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator++() noexcept -> Iterator&  // prefix ++
{
    moveIterator(1);
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator++(int) noexcept
  -> Iterator  // postfix ++
{
    auto temp = Iterator<ElementType, UnderlyingIteratorType>{*this};
    ++*this;
    return temp;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator--() noexcept -> Iterator&  // prefix --
{
    moveIterator(-1);
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator--(int) noexcept
  -> Iterator  // postfix --
{
    auto temp = Iterator<ElementType, UnderlyingIteratorType>{*this};
    --*this;
    return temp;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator+=(difference_type n) noexcept
  -> Iterator&
{
    moveIterator(n);
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator-=(difference_type n) noexcept
  -> Iterator&
{
    moveIterator(-n);
    return *this;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::operator[](difference_type offset) const noexcept
  -> reference
{
    return *(*this + offset);
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr bool Matrix::Iterator<ElementType, UnderlyingIteratorType>::isValid() const noexcept
{
    return m_currentI < m_rowsNumber && m_currentJ < m_columnsNumber;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr void Matrix::Iterator<ElementType, UnderlyingIteratorType>::setInvalidState() noexcept
{
    updateCurrentPosition(m_rowsNumber, 0);
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr void Matrix::Iterator<ElementType, UnderlyingIteratorType>::updateCurrentPosition(size_t i, size_t j) noexcept
{
    m_currentI = i;
    m_currentJ = j;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr auto Matrix::Iterator<ElementType, UnderlyingIteratorType>::distanceTo(const Iterator& j) const noexcept
  -> difference_type
{
    const auto thisPosition = size_t{m_currentI * m_columnsNumber + m_currentJ};
    const auto jPosition    = size_t{j.m_currentI * j.m_columnsNumber + j.m_currentJ};
    return thisPosition - jPosition;
}

template<typename ElementType, std::random_access_iterator UnderlyingIteratorType>
constexpr void Matrix::Iterator<ElementType, UnderlyingIteratorType>::moveIterator(difference_type n) noexcept
{
    m_dataIterator += n;

    const auto currentIndex = size_t{m_currentI * m_columnsNumber + m_currentJ};
    const auto newIndex     = size_t{currentIndex + n};

    if (newIndex <= m_maxIndex)
    {
        m_currentJ = newIndex % m_columnsNumber;
        m_currentI = (newIndex - m_currentJ) / m_columnsNumber;
    }
    else
    {
        setInvalidState();
    }
}

//------

Matrix::Matrix(size_t rowsNumber, size_t columnsNumber, float defaultValue) :
    m_columnsNumber{columnsNumber}, m_rowsNumber{rowsNumber}
{
    if (m_rowsNumber == 0 || m_columnsNumber == 0)
    {
        throw ogls::exceptions::MatrixException{formatZeroDimensionErrorMessage(m_rowsNumber, m_columnsNumber)};
    }

    m_data = std::vector(m_rowsNumber * m_columnsNumber, defaultValue);
}

Matrix::Matrix(const Size& size, float defaultValue) : Matrix(size.rows, size.columns, defaultValue)
{
}

Matrix::Matrix(std::initializer_list<std::initializer_list<float>> values)
{
    m_rowsNumber = values.size();
    if (m_rowsNumber == 0)
    {
        throw ogls::exceptions::MatrixException{formatZeroDimensionErrorMessage(0, 0)};
    }

    for (const auto& column : values)
    {
        m_columnsNumber = column.size();
        break;
    }

    initMatrixFromContainerOfContainers(m_rowsNumber, m_columnsNumber, m_data, values);
}

Matrix::Matrix(size_t rowsNumber, size_t columnsNumber, std::initializer_list<float> values) :
    m_columnsNumber{columnsNumber}, m_rowsNumber{rowsNumber}
{
    initMatrixFromContainer(m_rowsNumber, m_columnsNumber, m_data, values);
}

Matrix::Matrix(const std::vector<std::vector<float>>& values)
{
    m_rowsNumber = values.size();
    if (m_rowsNumber == 0)
    {
        throw ogls::exceptions::MatrixException{formatZeroDimensionErrorMessage(0, 0)};
    }

    m_columnsNumber = values.at(0).size();

    initMatrixFromContainerOfContainers(m_rowsNumber, m_columnsNumber, m_data, values);
}

Matrix::Matrix(size_t rowsNumber, size_t columnsNumber, const std::vector<float>& values) :
    m_columnsNumber{columnsNumber}, m_rowsNumber{rowsNumber}
{
    initMatrixFromContainer(m_rowsNumber, m_columnsNumber, m_data, values);
}

Matrix Matrix::operator-() const
{
    if (isNullMatrix())
    {
        return Matrix{};
    }

    auto result = Matrix{*this};
    // clang-format off
    result.performOnEvery([](Index, float element) { return -element; });  // clang-format on
    return result;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
    if (this->size() != m.size())
    {
        throw ogls::exceptions::MatricesDimensionalityException{this->toSizeOnlyString(), m.toSizeOnlyString(),
                                                                "operator+="};
    }

    // clang-format off
    this->performOnEvery([&](Index pos, float element) { return element + m.getValue(pos); });  // clang-format on
    return *this;
}

Matrix& Matrix::operator+=(float num) noexcept
{
    // clang-format off
    this->performOnEvery([=](Index, float element) { return element + num; });  // clang-format on
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& m)
{
    if (this->size() != m.size())
    {
        throw ogls::exceptions::MatricesDimensionalityException{this->toSizeOnlyString(), m.toSizeOnlyString(),
                                                                "operator-="};
    }

    // clang-format off
    this->performOnEvery([&](Index pos, float element) { return element - m.getValue(pos); });  // clang-format on
    return *this;
}

Matrix& Matrix::operator-=(float num) noexcept
{
    // clang-format off
    this->performOnEvery([=](Index, float element) { return element - num; });  // clang-format on
    return *this;
}

Matrix& Matrix::operator*=(float num) noexcept
{
    // clang-format off
    this->performOnEvery([=](Index, float element) { return element * num; });  // clang-format on
    return *this;
}

Matrix& Matrix::operator/=(float num)
{
    if (num == 0.0f)
    {
        throw ogls::exceptions::DivisionByZeroException{std::format("{} /= 0.0", this->toSizeOnlyString())};
    }

    // clang-format off
    this->performOnEvery([=](Index, float element) { return element / num; });  // clang-format on
    return *this;
}

Matrix::iterator Matrix::begin() noexcept
{
    return iterator{m_rowsNumber, m_columnsNumber, m_data.begin()};
}

Matrix::const_iterator Matrix::begin() const noexcept
{
    return const_iterator{m_rowsNumber, m_columnsNumber, m_data.begin()};
}

Matrix::const_iterator Matrix::cbegin() const noexcept
{
    return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cbegin()};
}

Matrix::iterator Matrix::end() noexcept
{
    return iterator{m_rowsNumber, m_columnsNumber, m_data.end(), true};
}

Matrix::const_iterator Matrix::end() const noexcept
{
    return const_iterator{m_rowsNumber, m_columnsNumber, m_data.end(), true};
}

Matrix::const_iterator Matrix::cend() const noexcept
{
    return const_iterator{m_rowsNumber, m_columnsNumber, m_data.cend(), true};
}

float Matrix::getValue(size_t row, size_t column) const
{
    if (row >= m_rowsNumber || column >= m_columnsNumber)
    {
        throw std::out_of_range{formatInvalidElementPositionErrorMessage(m_rowsNumber, m_columnsNumber,
                                                                         Index{.rows = row, .columns = column})};
    }
    return m_data[row * m_columnsNumber + column];
}

float Matrix::getValue(const Index& elementPosition) const
{
    return getValue(elementPosition.rows, elementPosition.columns);
}

bool Matrix::isIdentityMatrix() const noexcept
{
    if (isNullMatrix() || m_rowsNumber != m_columnsNumber)
    {
        return false;  // Identity matrices must be square
    }

    for (const auto& el : *this)
    {
        if (el.i != el.j)
        {
            if (el.getValue() != 0.0f)
            {
                return false;  // Non-diagonal elements must be 0
            }
        }
        else
        {
            if (el.getValue() != 1.0f)
            {
                return false;  // Diagonal elements must be 1
            }
        }
    }

    return true;
}

bool Matrix::isMatrixOfOnes() const noexcept
{
    if (isNullMatrix())
    {
        return false;
    }

    for (const auto element : m_data)
    {
        if (element != 1.0f)
        {
            return false;
        }
    }

    return true;
}

bool Matrix::isZeroMatrix() const noexcept
{
    if (isNullMatrix())
    {
        return false;
    }

    for (const auto element : m_data)
    {
        if (element != 0.0f)
        {
            return false;
        }
    }

    return true;
}

void Matrix::performOnEvery(std::function<float(Index, float)> functor)
{
    if (isNullMatrix())
    {
        return;
    }

    for (auto i = size_t{0}; i < m_rowsNumber; ++i)
    {
        for (auto j = size_t{0}; j < m_columnsNumber; ++j)
        {
            const auto index = i * m_columnsNumber + j;
            m_data[index]    = functor(Index{.rows = i, .columns = j}, m_data[index]);
        }
    }
}

void Matrix::performOnEvery(std::function<void(Index, float)> functor) const
{
    if (isNullMatrix())
    {
        return;
    }

    for (auto i = size_t{0}; i < m_rowsNumber; ++i)
    {
        for (auto j = size_t{0}; j < m_columnsNumber; ++j)
        {
            functor(Index{.rows = i, .columns = j}, m_data[i * m_columnsNumber + j]);
        }
    }
}

void Matrix::setValue(size_t row, size_t column, float value)
{
    if (row >= m_rowsNumber || column >= m_columnsNumber)
    {
        throw std::out_of_range{formatInvalidElementPositionErrorMessage(m_rowsNumber, m_columnsNumber,
                                                                         Index{.rows = row, .columns = column})};
    }
    m_data[row * m_columnsNumber + column] = value;
}

void Matrix::setValue(const Index& elementPosition, float value)
{
    setValue(elementPosition.rows, elementPosition.columns, value);
}

std::string Matrix::toFullString(int columnWidth) const
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
    ss << "  | " << std::left << std::setw(columnWidth) << (*el).getValue() << ", ";

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

std::string Matrix::toSizeOnlyString() const
{
    return std::format("Matrix {}x{}", m_rowsNumber, m_columnsNumber);
}

bool operator==(const Matrix& m1, const Matrix& m2) noexcept
{
    if (m1.size() != m2.size())
    {
        return false;
    }
    if (m1.isNullMatrix())
    {
        return true;
    }

    auto result = true;
    m1.performOnEvery(
      [&](Matrix::Index pos, float element)
      {
          if (helpers::isFloatsNotEqual(element, m2.getValue(pos)))
          {
              result = false;
          }
      });

    return result;
}

Matrix operator+(const Matrix& m1, const Matrix& m2)
{
    if (m1.size() != m2.size())
    {
        throw ogls::exceptions::MatricesDimensionalityException{m1.toSizeOnlyString(), m2.toSizeOnlyString(),
                                                                "operator+"};
    }
    if (m1.isNullMatrix())
    {
        return Matrix{};
    }

    auto result = Matrix{m1};
    // clang-format off
    result.performOnEvery([&](Matrix::Index pos, float element) { return element + m2.getValue(pos); });  // clang-format on
    return result;
}

Matrix operator+(float num, const Matrix& m)
{
    if (m.isNullMatrix())
    {
        return Matrix{};
    }

    auto result  = Matrix{m};
    result      += num;
    return result;
}

Matrix operator-(const Matrix& m1, const Matrix& m2)
{
    if (m1.size() != m2.size())
    {
        throw ogls::exceptions::MatricesDimensionalityException{m1.toSizeOnlyString(), m2.toSizeOnlyString(),
                                                                "operator-"};
    }
    if (m1.isNullMatrix())
    {
        return Matrix{};
    }

    auto result = Matrix{m1};
    // clang-format off
    result.performOnEvery([&](Matrix::Index pos, float element) { return element - m2.getValue(pos); });  // clang-format on
    return result;
}

Matrix operator-(const Matrix& m, float num)
{
    if (m.isNullMatrix())
    {
        return Matrix{};
    }

    auto result  = Matrix{m};
    result      -= num;
    return result;
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
    if (m1.getColumnCount() != m2.getRowCount())
    {
        throw ogls::exceptions::MatricesDimensionalityException{m1.toSizeOnlyString(), m2.toSizeOnlyString(),
                                                                "operator*",
                                                                "The number of columns of the Matrix m1 must be equal "
                                                                "to the number of rows of the Matrix m2."};
    }
    if (m1.isNullMatrix())
    {
        return Matrix{};
    }

    const auto resultSize = Matrix::Size{.rows = m1.getRowCount(), .columns = m2.getColumnCount()};
    auto       result     = Matrix{resultSize};

    for (auto i = size_t{0}; i < resultSize.rows; ++i)
    {
        for (auto j = size_t{0}; j < resultSize.columns; ++j)
        {
            auto resultElement = float{0.0};

            for (auto iInner = size_t{0}; iInner < m1.getColumnCount(); ++iInner)
            {
                resultElement += m1.getValue(i, iInner) * m2.getValue(iInner, j);
            }

            result.setValue(i, j, resultElement);
        }
    }

    return result;
}

Matrix operator*(float num, const Matrix& m)
{
    if (m.isNullMatrix())
    {
        return Matrix{};
    }

    auto result  = Matrix{m};
    result      *= num;
    return result;
}

Matrix operator/(const Matrix& m, float num)
{
    if (num == 0.0f)
    {
        throw ogls::exceptions::DivisionByZeroException{std::format("{} / 0.0", m.toSizeOnlyString())};
    }
    if (m.isNullMatrix())
    {
        return Matrix{};
    }

    auto result  = Matrix{m};
    result      /= num;
    return result;
}

namespace
{
    template<typename Type>
    void initMatrixFromContainer(size_t rowsNumber, size_t columnsNumber, std::vector<float>& matrixData,
                                 Type& container)
    {
        if (rowsNumber == 0 || columnsNumber == 0)
        {
            throw ogls::exceptions::MatrixException{formatZeroDimensionErrorMessage(rowsNumber, columnsNumber)};
        }
        if (container.size() < rowsNumber * columnsNumber)
        {
            throw ogls::exceptions::MatrixException{
              std::format("Invalid input container size. Input container must contain at least {} elements.",
                          rowsNumber * columnsNumber)};
        }

        matrixData = std::vector<float>{container.begin(), container.begin() + rowsNumber * columnsNumber};
    }

    template<typename Type>
    void initMatrixFromContainerOfContainers(size_t rowsNumber, size_t columnsNumber, std::vector<float>& matrixData,
                                             Type& container)
    {
        for (const auto& column : container)
        {
            if (column.size() != columnsNumber)
            {
                throw ogls::exceptions::MatrixException{"All columns must have the same size."};
            }
        }
        // It's ensured that the size of all columns is equal and, if it's zero, it seems to be real intention to create
        // Matrix with zero-sized columns, what is illegal.
        if (columnsNumber == 0)
        {
            throw ogls::exceptions::MatrixException{formatZeroDimensionErrorMessage(rowsNumber, 0)};
        }

        matrixData = std::vector(rowsNumber * columnsNumber, 0.0f);

        auto insertPos = matrixData.begin();
        for (const auto& column : container)
        {
            std::copy(column.begin(), column.end(), insertPos);
            insertPos += columnsNumber;
        }
    }

    std::string formatInvalidElementPositionErrorMessage(size_t rowsNumber, size_t columnsNumber,
                                                         const Matrix::Index& position)
    {
        return std::format("Matrix size is {}x{}, but passed element position is [{}][{}]", rowsNumber, columnsNumber,
                           position.rows, position.columns);
    }

    std::string formatZeroDimensionErrorMessage(size_t rowsNumber, size_t columnsNumber)
    {
        return std::format("Zero value for one or two of Matrix dimensions: rows - {}, columns - {}.", rowsNumber,
                           columnsNumber);
    }

    void throwUnexpectedUsageExceptionWithHint(const char* operatorName)
    {
        throw std::domain_error{
          std::format("Matrix::Iterator::{} is not allowed by the design of the Matrix class. Hint: attempting "
                      "to use Matrix::Iterator to modify the underlying Matrix data container outside of the "
                      "Matrix implementation by adding or removing container elements.",
                      operatorName)};
    }

}  // namespace

template class Matrix::Iterator<float, std::vector<float>::iterator>;
template class Matrix::Iterator<const float, std::vector<float>::const_iterator>;

}  // namespace ogls::mathCore
