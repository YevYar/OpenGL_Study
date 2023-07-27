#include "mathCore/matrix.h"

#include <algorithm>
#include <iomanip>
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

}  // namespace

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

    const auto size = m_rowsNumber;
    for (auto i = size_t{0}; i < size; ++i)
    {
        if (m_data[i * size + i] != 1.0f)
        {
            return false;  // Diagonal elements must be 1
        }

        for (auto j = size_t{0}; j < size; ++j)
        {
            if (i != j && m_data[i * size + j] != 0.0f)
            {
                return false;  // Non-diagonal elements must be 0
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

    for (auto i = size_t{0}; i < m_rowsNumber; ++i)
    {
        ss << "  | ";

        for (auto j = size_t{0}; j < m_columnsNumber; ++j)
        {
            ss << std::left << std::setw(columnWidth) << m_data[i * m_columnsNumber + j];

            if (j != m_columnsNumber - 1)
            {
                ss << ", ";
            }
        }

        ss << " |";
        if (i != m_rowsNumber - 1)
        {
            ss << "\n";
        }
    }

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

}  // namespace

}  // namespace ogls::mathCore
