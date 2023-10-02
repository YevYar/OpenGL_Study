#ifndef OGLS_MATHCORE_VECTOR_H
#define OGLS_MATHCORE_VECTOR_H

#include <functional>

#include "exceptions.h"
#include "mathCore/matrix.h"
#include "mathCore/point.h"

namespace ogls::mathCore
{
/**
 * \brief Vector represents a vector in 3D (2D) orthonormal basis.
 */
class Vector
{
    public:
        /**
         * \brief Constructs zero-vector.
         */
        constexpr Vector() noexcept = default;
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(Vector)

        /**
         * \brief Constructs Vector with passed Vector coordinates X, Y and Z.
         */
        constexpr Vector(float x, float y, float z = {0.0f}) noexcept : m_x{x}, m_y{y}, m_z{z}
        {
        }

        /**
         * \brief Constructs Vector from two Point.
         *
         * \param p1 - a start point of the Vector.
         * \param p2 - an end point of the Vector.
         */
        constexpr Vector(const Point& p1, const Point& p2) noexcept :
            m_x{p2.x - p1.x}, m_y{p2.y - p1.y}, m_z{p2.z - p1.z}
        {
        }

        /**
         * \brief Constructs Vector with all coordinates set to the value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr explicit Vector(float generalCoordinate) noexcept :
            m_x{generalCoordinate}, m_y{generalCoordinate}, m_z{generalCoordinate}
        {
        }

        //------ SOME UNARY OPERATIONS

        /**
         * \brief Returns new Vector as the result of negation of this Vector.
         */
        constexpr Vector operator-() const noexcept
        {
            return Vector{-m_x, -m_y, -m_z};
        }

        /**
         * \brief Returns true if this Vector is zero-vector, false otherwise.
         *
         * \see isZeroVector().
         */
        constexpr bool operator!() const noexcept
        {
            return !bool{*this};
        }

        //------ SOME COMPOUND ASSIGNMENT OPERATIONS

        /**
         * \brief Adds another Vector to this Vector.
         *
         * Changes this Vector by performing addition of coordinates of v to coordinates of this Vector.
         *
         * \param v - another Vector to add to this Vector.
         * \return this Vector with changed coordinates.
         */
        constexpr Vector& operator+=(const Vector& v) noexcept
        {
            return changeCoordinatesViaOperand(v.m_x, v.m_y, v.m_z, std::plus<float>{});
        }

        /**
         * \brief Adds number to all coordinates of this Vector.
         *
         * Changes this Vector by performing addition of the number to all coordinates of this Vector.
         *
         * \param num - a number to add to all coordinates of this Vector.
         * \return this Vector with changed coordinates.
         */
        constexpr Vector& operator+=(float num) noexcept
        {
            return changeCoordinatesViaOperand(num, num, num, std::plus<float>{});
        }

        /**
         * \brief Subtracts another Vector from this Vector.
         *
         * Changes this Vector by performing subtraction of coordinates of v from coordinates of this Vector.
         *
         * \param v - another Vector to subtract from this Vector.
         * \return this Vector with changed coordinates.
         */
        constexpr Vector& operator-=(const Vector& v) noexcept
        {
            return changeCoordinatesViaOperand(v.m_x, v.m_y, v.m_z, std::minus<float>{});
        }

        /**
         * \brief Subtracts number from all coordinates of this Vector.
         *
         * Changes this Vector by performing subtraction of the number from all coordinates of this Vector.
         *
         * \param num - a number to subtract from all coordinates of this Vector.
         * \return this Vector with changed coordinates.
         */
        constexpr Vector& operator-=(float num) noexcept
        {
            return changeCoordinatesViaOperand(num, num, num, std::minus<float>{});
        }

        /**
         * \brief Multiplies all coordinates of this Vector by number.
         *
         * Changes this Vector by performing multiplication of all coordinates of this Vector by number.
         *
         * \param num - a number to multiply coordinates of this Vector by.
         * \return this Vector with changed coordinates.
         */
        constexpr Vector& operator*=(float num) noexcept
        {
            return changeCoordinatesViaOperand(num, num, num, std::multiplies<float>{});
        }

        /**
         * \brief Divides all coordinates of this Vector by number.
         *
         * Changes this Vector by performing division of all coordinates of this Vector by number.
         *
         * \param num - a number to divide coordinates of this Vector by.
         * \return this Vector with changed coordinates.
         * \throw ogls::exceptions::DivisionByZeroException().
         */
        constexpr Vector& operator/=(float num)
        {
            if (num == 0.0f)
            {
                throw ogls::exceptions::DivisionByZeroException{
                  std::format("{} /= 0.0", static_cast<std::string>(*this))};
            }

            return changeCoordinatesViaOperand(num, num, num, std::divides<float>{});
        }

        //------ SOME TYPE CASTING OPERATIONS

        /**
         * \brief Returns boolean representation of the Vector object.
         *
         * \see isZeroVector().
         * \return false if the Vector is zero-vector, true otherwise.
         */
        constexpr explicit operator bool() const noexcept
        {
            return !isZeroVector();
        }

        /**
         * \brief Returns std::string representation of the Vector object.
         */
        explicit operator std::string() const
        {
            return std::format("Vector(x={}, y={}, z={} | length={})", m_x, m_y, m_z, length());
        }

        //------

        /**
         * \brief Checks if the Vector is normalized vector (unit vector, length is equal to 1.0).
         *
         * \return true if the Vector is unit-vector, false otherwise.
         */
        constexpr bool isNormalized() const noexcept
        {
            // Check approximately, because it's rare case, when actually normalized Vector has length exactly 1.0.
            return ogls::helpers::isFloatsEqual(length(), 1.0f);
        }

        /**
         * \brief Checks if the Vector is zero-vector (length is equal to 0.0).
         *
         * \return true if the Vector is zero-vector, false otherwise.
         */
        constexpr bool isZeroVector() const noexcept
        {
            return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f;
        }

        /**
         * \brief Returns the length (magnitude) of the Vector.
         */
        constexpr float length() const noexcept
        {
            return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f ? 0.0f
                                                             : std::sqrtf(square(m_x) + square(m_y) + square(m_z));
        }

        /**
         * \brief Sets the X coordinate of the Vector.
         *
         * \param x - value to set.
         */
        constexpr void setX(float x) noexcept
        {
            m_x = x;
        }

        /**
         * \brief Sets the Y coordinate of the Vector.
         *
         * \param y - value to set.
         */
        constexpr void setY(float y) noexcept
        {
            m_y = y;
        }

        /**
         * \brief Sets the Z coordinate of the Vector.
         *
         * \param z - value to set.
         */
        constexpr void setZ(float z) noexcept
        {
            m_z = z;
        }

        /**
         * \brief Sets the X, Y, Z coordinates of the Vector.
         *
         * \param x, y, z - values to set.
         */
        constexpr void setCoordinates(float x, float y, float z = {0.0f}) noexcept
        {
            m_x = x;
            m_y = y;
            m_z = z;
        }

        /**
         * \brief Sets the X, Y, Z coordinates of the Vector to the value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr void setCoordinates(float generalCoordinate) noexcept
        {
            m_x = generalCoordinate;
            m_y = generalCoordinate;
            m_z = generalCoordinate;
        }

        /**
         * \brief Returns a std::string representation of the Vector object.
         */
        std::string toString() const
        {
            return static_cast<std::string>(*this);
        }

        /**
         * \brief Returns X coordinate of the Vector.
         */
        constexpr float x() const noexcept
        {
            return m_x;
        }

        /**
         * \brief Returns Y coordinate of the Vector.
         */
        constexpr float y() const noexcept
        {
            return m_y;
        }

        /**
         * \brief Returns Z coordinate of the Vector.
         */
        constexpr float z() const noexcept
        {
            return m_z;
        }

    private:
        /**
         * \brief Returns this Vector with changed coordinates in the result of applying of operand to all coordinates
         * of the Vector.
         */
        constexpr Vector& changeCoordinatesViaOperand(float x, float y, float z, auto operand)
        {
            m_x = operand(m_x, x);
            m_y = operand(m_y, y);
            m_z = operand(m_z, z);
            return *this;
        }

    private:
        float m_x = {0.0f}, m_y = {0.0f}, m_z = {0.0f};

};  // class Vector

/**
 * VectorIntoMatrixInsertionOrder defines in which order the Vector elements are inserted into the Matrix.
 */
enum class VectorIntoMatrixInsertionOrder
{
    ColumnMajor,
    RowMajor
};

//------ OPERATIONS ON VECTOR

/**
 * \brief Calculates a dot product of two Vector by their coordinates.
 *
 * \return the dot product of two Vector.
 */
constexpr float dotProduct(const Vector& v1, const Vector& v2) noexcept
{
    return (v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z());
}

/**
 * \brief Calculates a dot product of two Vector by their lengths and an angle between them.
 *
 * \param lengthV1 - length (magnitude) of Vector 1.
 * \param lengthV2 - length (magnitude) of Vector 2.
 * \param angle    - angle between two Vector.
 * \param unit     - measurement unit of the angle.
 * \return the dot product of two Vector.
 */
inline float dotProduct(float lengthV1, float lengthV2, float angle, AngleUnit unit = AngleUnit::Degrees)
{
    return lengthV1 * lengthV2 * mathCore::cos(angle, unit);
}

/**
 * \brief Calculates a dot product of two Vector by their lengths and an angle between them.
 *
 * \param v1    - Vector 1.
 * \param v2    - Vector 2.
 * \param angle - angle between two Vector.
 * \param unit  - measurement unit of the angle.
 * \return the dot product of two Vector.
 */
inline float dotProduct(const Vector& v1, const Vector& v2, float angle, AngleUnit unit = AngleUnit::Degrees)
{
    return v1.length() * v2.length() * mathCore::cos(angle, unit);
}

/**
 * \brief Calculates the value of cosine between two Vector.
 *
 * \return the value of cosine between two Vector.
 */
constexpr float cosBetweenVectors(const Vector& v1, const Vector& v2) noexcept
{
    const auto l1 = v1.length(), l2 = v2.length();
    // The angle between vector and zero-vector is 90 degrees, cos(90) = 0
    return l1 == 0.0f || l2 == 0.0f ? 0.0f : dotProduct(v1, v2) / l1 / l2;
}

/**
 * \brief Calculates the angle between two Vector.
 *
 * \return the angle in radians between two Vector.
 */
inline float angleBetweenVectors(const Vector& v1, const Vector& v2)
{
    return std::acosf(mapValueToUnitRange(cosBetweenVectors(v1, v2)));
}

/**
 * \brief Inserts the Vector into the Matrix in some order.
 *
 * Throws if the index is out of range of the Matrix.
 *
 * \param m           - the Matrix, in which the Vector is inserted.
 * \param v           - the Vector to insert.
 * \param order       - the order, in which elements of the Vector are inserted into the Matrix.
 * \param index       - the index of the row/column, in which the elements of the Vector are inserted.
 * \param placeholder - the value, which is inserted in the row/column of the Matrix, if it has size > 3
 * (size of the Vector).
 * \throw std::out_of_range.
 */
template<size_t N, size_t M, typename = IsNotNullMatrix<N, M>>
constexpr void insertVectorIntoMatrix(Matrix<N, M>& m, const Vector& v, VectorIntoMatrixInsertionOrder order,
                                      size_t index, float placeholder = {0.0f})
{
    const auto inserter = [&](size_t k, float value)
    {
        if (order == VectorIntoMatrixInsertionOrder::RowMajor)
        {
            m.setValue(index, k, value);
        }
        else
        {
            m.setValue(k, index, value);
        };
    };

    const auto maxIndex = order == VectorIntoMatrixInsertionOrder::RowMajor ? M : N;
    for (auto i = size_t{0}; i < maxIndex; ++i)
    {
        switch (i)
        {
            case 0:
                inserter(0, v.x());
                break;
            case 1:
                inserter(1, v.y());
                break;
            case 2:
                inserter(2, v.z());
                break;
            default:
                inserter(i, placeholder);
        }
    }
}

/**
 * \brief Calculates a cross product of two Vector.
 *
 * \param v1 - Vector 1.
 * \param v2 - Vector 2.
 * \return the cross product of two Vector.
 */
constexpr Vector crossProduct(const Vector& v1, const Vector& v2) noexcept
{
    auto m = Matrix<3, 3>{1.0f};

    insertVectorIntoMatrix(m, v1, VectorIntoMatrixInsertionOrder::RowMajor, 1);
    insertVectorIntoMatrix(m, v2, VectorIntoMatrixInsertionOrder::RowMajor, 2);

    return Vector{m.calculateAlgebraicComplement(BaseMatrix::Index{.rows = 0, .columns = 0}),
                  m.calculateAlgebraicComplement(BaseMatrix::Index{.rows = 0, .columns = 1}),
                  m.calculateAlgebraicComplement(BaseMatrix::Index{.rows = 0, .columns = 2})};
}

/**
 * \brief Creates new normalized vector of the passed vector.
 *
 * \param v - the Vector to normalize.
 * \return normalized Vector of the Vector v or zero-vector if v is zero-vector.
 */
constexpr Vector normalize(const Vector& v) noexcept
{
    const auto length = v.length();
    return length == 0.0f ? v : Vector{v.x() / length, v.y() / length, v.z() / length};
}

/**
 * \brief Checks equality of two Vector.
 *
 * \return true if two Vector have equal coordinates, false otherwise.
 */
constexpr bool operator==(const Vector& v1, const Vector& v2) noexcept
{
    using ogls::helpers::isFloatsEqual;


    return isFloatsEqual(v1.x(), v2.x()) && isFloatsEqual(v1.y(), v2.y()) && isFloatsEqual(v1.z(), v2.z());
}

/**
 * \brief Checks if two Vector are not equal.
 *
 * \return true if two Vector have different coordinates, false otherwise.
 */
constexpr bool operator!=(const Vector& v1, const Vector& v2) noexcept
{
    return !(v1 == v2);
}

/**
 * \brief Adds two Vector.
 *
 * \return Vector, which is the result of addition of two Vector.
 */
constexpr Vector operator+(const Vector& v1, const Vector& v2) noexcept
{
    return Vector{v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

/**
 * \brief Adds number to a Vector.
 *
 * \param num - a number to add to every coordinate of the Vector.
 * \param v   - a source Vector.
 * \return new Vector, which is a result of addition of the number to every coordinate of Vector v.
 */
constexpr Vector operator+(float num, const Vector& v) noexcept
{
    return v + Vector{num, num, num};
}

/**
 * \brief Adds number to a Vector.
 *
 * \param v   - a source Vector.
 * \param num - a number to add to every coordinate of the Vector.
 * \return new Vector, which is a result of addition of the number to every coordinate of Vector v.
 */
constexpr Vector operator+(const Vector& v, float num) noexcept
{
    return num + v;
}

/**
 * \brief Subtracts two Vector.
 *
 * \return Vector, which is the result of subtraction of two Vector.
 */
constexpr Vector operator-(const Vector& v1, const Vector& v2) noexcept
{
    return Vector{v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

/**
 * \brief Subtracts the number from the Vector.
 *
 * \param v   - a source Vector.
 * \param num - a number to subtract from every coordinate of the Vector.
 * \return new Vector, which is a result of subtraction of the number from every coordinate of Vector v.
 */
constexpr Vector operator-(const Vector& v, float num) noexcept
{
    return v - Vector{num, num, num};
}

/**
 * \brief Multiplies all coordinates of the Vector by number.
 *
 * \param num - a number to multiply coordinates of the Vector by.
 * \param v   - a source Vector.
 * \return new Vector, which is a result of multiplication of every coordinate of Vector v by number.
 */
constexpr Vector operator*(float num, const Vector& v) noexcept
{
    return Vector{v.x() * num, v.y() * num, v.z() * num};
}

/**
 * \brief Multiplies all coordinates of the Vector by number.
 *
 * \param v   - a source Vector.
 * \param num - a number to multiply coordinates of the Vector by.
 * \return new Vector, which is a result of multiplication of every coordinate of Vector v by number.
 */
constexpr Vector operator*(const Vector& v, float num) noexcept
{
    return num * v;
}

/**
 * \brief Divides all coordinates of the Vector by number.
 *
 * \param v   - a source Vector.
 * \param num - a number to divide coordinates of the Vector by.
 * \return new Vector, which is a result of division of every coordinate of Vector v by number.
 * \throw ogls::exceptions::DivisionByZeroException().
 */
constexpr Vector operator/(const Vector& v, float num)
{
    if (num == 0.0f)
    {
        throw ogls::exceptions::DivisionByZeroException{std::format("{} / {}", std::string{v}, num)};
    }

    return Vector{v.x() / num, v.y() / num, v.z() / num};
}

//------ CHECKERS OF VECTOR PROPERTIES

/**
 * \brief Checks if two Vector are co-directed.
 *
 * \return true if co-directed, false otherwise.
 */
inline bool isVectorsCodirected(const Vector& v1, const Vector& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), 0.0f);
}

/**
 * \brief Checks if two Vector are collinear.
 *
 * \return true if collinear, false otherwise.
 */
inline bool isVectorsCollinear(const Vector& v1, const Vector& v2)
{
    return crossProduct(v1, v2).isZeroVector();
}

/**
 * \brief Checks if two Vector are oppositely directed (the angle between Vector is 180 degrees).
 *
 * \return true if oppositely directed, false otherwise.
 */
inline bool isVectorsOppositelyDirected(const Vector& v1, const Vector& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), std::numbers::pi);
}

/**
 * \brief Checks if two Vector are orthogonal.
 *
 * \return true if orthogonal, false otherwise.
 */
constexpr bool isVectorsOrthogonal(const Vector& v1, const Vector& v2) noexcept
{
    return helpers::isFloatsEqual(dotProduct(v1, v2), 0.0f);
}

}  // namespace ogls::mathCore

#endif
