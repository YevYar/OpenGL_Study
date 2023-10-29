#ifndef OGLS_MATHCORE_VECTOR_H
#define OGLS_MATHCORE_VECTOR_H

#include <functional>

#include "exceptions.h"
#include "mathCore/matrix.h"
#include "mathCore/point.h"

namespace ogls::mathCore
{
template<typename OtherIterator>
concept OtherVectorIterator = requires(OtherIterator obj) {
    // clang-format off
    { obj.equalToOtherIterator(0, nullptr) } -> std::same_as<bool>;  // clang-format on
};

/**
 * \brief Vector represents a vector in 3D (2D) orthonormal basis.
 *
 * It has W-component too.
 *
 * \see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates).
 */
class Vector
{
    public:
        /**
         * \brief Vector::Iterator provides an access to Vector components.
         *
         * Iterator implements std::forward_iterator interface. Dereference operator of the Iterator returns
         * an Component object, which provides an access to the pointed component of the Vector.
         *
         * Iterator follows the convention of the STL iterators to not throw an exception, when its user tries to
         * dereference invalid Iterator (see isValid()). However move operations as well as copy assignment
         * throw an exception to prevent unexpected usage of such algorithms as std::remove() on the Iterator.
         *
         * \param ComponentType - the type of referenced components (float, const float).
         * \see isValid().
         */
        template<typename ComponentType>
        class Iterator final
        {
            public:
                /**
                 * \brief Component keeps the data of the Vector component, on which the Iterator points.
                 */
                struct Component
                {
                    public:
                        /**
                         * \brief Allows copy assignment of the float value.
                         *
                         * Changes the referenced Vector component. Makes it possible to use algorithms like std::fill().
                         */
                        template<typename = std::enable_if_t<!std::is_const_v<ComponentType>>>
                        constexpr Component& operator=(ComponentType newValue) noexcept
                        {
                            setValue(newValue);
                            return *this;
                        }

                        /**
                         * \brief Returns an ComponentType representation of the Component object.
                         *
                         * Makes it possible to use algorithms like std::accumulate().
                         *
                         * \see getValue().
                         */
                        constexpr operator ComponentType() const noexcept
                        {
                            return getValue();
                        }

                        /**
                         * \brief Returns a std::string representation of the Component object.
                         */
                        operator std::string() const
                        {
                            auto component = ' ';
                            switch (i)
                            {
                                case 0:
                                    component = 'X';
                                    break;
                                case 1:
                                    component = 'Y';
                                    break;
                                case 2:
                                    component = 'Z';
                                    break;
                                case 3:
                                    component = 'W';
                                    break;
                                default:
                                    OGLS_ASSERT(false);
                                    return "Vector::Iterator::Unknown Component";
                            }
                            return std::format("Vector::Iterator::{} = {}", component, getValue());
                        }

                        /**
                         * \brief Returns the reference on the referenced Vector component.
                         *
                         * Use it to change the referenced Vector component: getValue() = newValue;
                         */
                        constexpr ComponentType& getValue() const noexcept
                        {
                            OGLS_ASSERT(value != nullptr);
                            return *value;
                        }

                        /**
                         * \brief Sets the new value of the referenced component.
                         *
                         * Changes the referenced Vector component.
                         */
                        template<typename = std::enable_if_t<!std::is_const_v<ComponentType>>>
                        constexpr void setValue(ComponentType newValue) noexcept
                        {
                            getValue() = newValue;
                        }

                    public:
                        /**
                         * \brief The index of the Vector component.
                         */
                        size_t         i     = {4};
                        /**
                         * \brief The pointer to the value of the Component.
                         */
                        ComponentType* value = nullptr;

                };  // struct Component

                using VectorType = std::conditional_t<std::is_const_v<ComponentType>, const Vector, Vector>;

                // The following type aliases are STL conventions
                using difference_type   = std::ptrdiff_t;
                using iterator_category = std::forward_iterator_tag;
                using iterator_concept  = std::forward_iterator_tag;
                using value_type        = Component;
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
                 */
                constexpr Iterator(const Iterator&) noexcept = default;  // it's required by iterator concept

                /**
                 * \brief Constructs the Iterator for the provided Vector.
                 *
                 * \param vec           - the Vector, over the components of to iterate.
                 * \param isEndIterator - indicator, which is necessary for correct initialisation of the Iterator.
                 * If true the Iterator is initialised with a value 4. If false, the Iterator is initialised with a
                 * value 0.
                 * \see isValid().
                 */
                constexpr Iterator(VectorType* vec, bool isEndIterator = false) noexcept :
                    m_i{0}, m_isDefaultInitialised{false}, m_vec{vec}
                {
                    if (isEndIterator)
                    {
                        setInvalidState();
                    }
                }

                /**
                 * \brief Constructs new Iterator as move-copy of the other Iterator.
                 *
                 * \throw ogls::exceptions::VectorException().
                 */
                constexpr Iterator(Iterator&&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("Iterator(Iterator&&)");
                }

                /**
                 * \brief Copies the state of the other Iterator.
                 *
                 * \throw ogls::exceptions::VectorException().
                 */
                constexpr Iterator& operator=(const Iterator&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("operator=(const Iterator&)");
                    return *this;
                }

                /**
                 * \brief Move-copies the state of the other Iterator.
                 *
                 * \throw ogls::exceptions::VectorException().
                 */
                constexpr Iterator& operator=(Iterator&&)  // it's required by iterator concept
                {
                    throwUnexpectedUsageExceptionWithHint("operator=(Iterator&&)");
                    return *this;
                }

                // concept forward_iterator
                constexpr reference operator*() const noexcept
                {
                    auto value = &m_vec->m_x;
                    switch (m_i)
                    {
                        case 0:
                            value = &m_vec->m_x;
                            break;
                        case 1:
                            value = &m_vec->m_y;
                            break;
                        case 2:
                            value = &m_vec->m_z;
                            break;
                        case 3:
                            value = &m_vec->m_w;
                            break;
                        default:
                            OGLS_ASSERT(false);
                            value = nullptr;
                    }

                    return isValid() && value ? Component{.i = m_i, .value = value}
                                              : Component{.i = 4, .value = nullptr};
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
                 * \brief Checks equality of the object of this Iterator type and any other Vector::Iterator.
                 */
                template<OtherVectorIterator OtherIterator>
                friend constexpr bool operator==(const Iterator& i, const OtherIterator& j) noexcept
                {
                    // This trick is used, because this operator== cannot be a friend to both Iterator and OtherIterator
                    // at the same time. So the public method of OtherIterator is called to compare the data of the
                    // object j with the data of the object i, for which operator== is a friend and has an access to
                    // its data.
                    return j.equalToOtherIterator(i.m_i, i.m_vec);
                }

                /**
                 * \brief Checks non-equality of two Iterator.
                 */
                template<OtherVectorIterator OtherIterator>
                friend constexpr bool operator!=(const Iterator& i, const OtherIterator& j) noexcept
                {
                    return !(i == j);
                }

                /**
                 * \brief Checks equality of the own data with the data of other Iterator.
                 */
                constexpr bool equalToOtherIterator(size_t index, const Vector* vec) const noexcept
                {
                    return m_vec == vec && m_i == index;
                }

                /**
                 * \brief Checks if Iterator is in valid state.
                 *
                 * An invalid state of the Iterator means that the Iterator doesn't point on any component of the Vector.
                 *
                 * \return true if Iterator is in valid state, false otherwise.
                 */
                constexpr bool isValid() const noexcept
                {
                    return !m_isDefaultInitialised && m_vec && m_i < 4;
                }

            private:
                /**
                 * \brief Sets the invalid state of the Iterator.
                 *
                 * \see isValid().
                 */
                constexpr void setInvalidState() noexcept
                {
                    m_i = {4};
                }

                /**
                 * \brief Moves Iterator on n positions.
                 *
                 * It must be equal to n calls of increment operator.
                 *
                 * \param n - an offset from the current Iterator position to other component.
                 */
                constexpr void moveIterator(difference_type n) noexcept
                {
                    const auto newIndex = size_t{m_i + n};

                    if (newIndex < 4)
                    {
                        m_i = newIndex;
                    }
                    else
                    {
                        setInvalidState();
                    }
                }

                void throwUnexpectedUsageExceptionWithHint(const char* operatorName) const
                {
                    throw ogls::exceptions::VectorException{
                      std::format("Vector::Iterator::{} is not allowed by the design of the Vector class.",
                                  operatorName)};
                }

            private:
                /**
                 * \brief The index of the Vector component.
                 *
                 * 0 - X component of the Vector.<br>
                 * 1 - Y component of the Vector.<br>
                 * 2 - Z component of the Vector.<br>
                 * 3 - W component of the Vector.<br>
                 */
                size_t      m_i                    = {4};  // invalid state
                bool        m_isDefaultInitialised = true;
                /**
                 * \brief The Vector, over the components of to iterate.
                 */
                VectorType* m_vec                  = nullptr;

        };  // class Iterator

        // The following type aliases are the conventions of STL

        /**
         * \brief Iterates over Vector components without providing ability to change the referenced Vector component.
         */
        using const_iterator = Iterator<const float>;
        /**
         * \brief Iterates over Vector components.
         */
        using iterator       = Iterator<float>;

    public:
        /**
         * \brief Constructs zero-vector.
         */
        constexpr Vector() noexcept = default;
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(Vector)

        /**
         * \brief Constructs Vector with passed Vector coordinates X, Y and Z and W-component.
         *
         * \see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates).
         */
        constexpr Vector(float x, float y, float z = {0.0f}, float w = {1.0f}) noexcept : m_x{x}, m_y{y}, m_z{z}, m_w{w}
        {
        }

        /**
         * \brief Constructs Vector from two Point.
         *
         * \note W-component is set to 1.
         * \param p1 - a start point of the Vector.
         * \param p2 - an end point of the Vector.
         */
        constexpr Vector(const Point& p1, const Point& p2) noexcept :
            m_x{p2.x - p1.x}, m_y{p2.y - p1.y}, m_z{p2.z - p1.z}, m_w{1.0f}
        {
        }

        /**
         * \brief Constructs Vector with all coordinates set to the value of generalCoordinate.
         *
         * \note W-component is set to 1.
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr explicit Vector(float generalCoordinate) noexcept :
            m_x{generalCoordinate}, m_y{generalCoordinate}, m_z{generalCoordinate}, m_w{1.0f}
        {
        }

        //------ SOME UNARY OPERATIONS

        /**
         * \brief Returns new Vector as the result of negation of this Vector.
         *
         * \note W-component is not affected.
         */
        constexpr Vector operator-() const noexcept
        {
            return Vector{-m_x, -m_y, -m_z, m_w};
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
            return std::format("Vector(x={}, y={}, z={}, w={} | length={})", m_x, m_y, m_z, m_w, length());
        }

        //------ RANGE STUFF

        /**
         * \brief Returns an Iterator to the beginning.
         */
        constexpr iterator begin() noexcept
        {
            return iterator{this};
        }

        /**
         * \brief Returns const Iterator to the beginning.
         */
        constexpr const_iterator begin() const noexcept
        {
            return const_iterator{this};
        }

        /**
         * \brief Returns const Iterator to the beginning.
         */
        constexpr const_iterator cbegin() const noexcept
        {
            return const_iterator{this};
        }

        /**
         * \brief Returns an Iterator to the end.
         */
        constexpr iterator end() noexcept
        {
            return iterator{this, true};
        }

        /**
         * \brief Returns const Iterator to the end.
         */
        constexpr const_iterator end() const noexcept
        {
            return const_iterator{this, true};
        }

        /**
         * \brief Returns const Iterator to the end.
         */
        constexpr const_iterator cend() const noexcept
        {
            return const_iterator{this, true};
        }

        /**
         * \brief Returns the number of components in the Vector.
         */
        constexpr size_t size() const noexcept
        {
            return 4;
        }

        //------

        /**
         * \brief Checks if the Vector is normalized vector (unit vector, length is equal to 1.0).
         *
         * \note W-component is not considered.
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
         * \note W-component is not considered.
         * \return true if the Vector is zero-vector, false otherwise.
         */
        constexpr bool isZeroVector() const noexcept
        {
            return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f;
        }

        /**
         * \brief Returns the length (magnitude) of the Vector.
         *
         * \note W-component is not taken into account.
         */
        constexpr float length() const noexcept
        {
            return m_x == 0.0f && m_y == 0.0f && m_z == 0.0f ? 0.0f
                                                             : std::sqrtf(square(m_x) + square(m_y) + square(m_z));
        }

        /**
         * \brief Sets the W-component of the Vector.
         *
         * \param w - value to set.
         */
        constexpr void setW(float w) noexcept
        {
            m_w = w;
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
         * \note W-component is not affected.
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
         * \note W-component is not affected.
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
         * \brief Returns W-component of the Vector.
         */
        constexpr float w() const noexcept
        {
            return m_w;
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
         *
         * \note W-component is not affected.
         */
        constexpr Vector& changeCoordinatesViaOperand(float x, float y, float z, auto operand)
        {
            m_x = operand(m_x, x);
            m_y = operand(m_y, y);
            m_z = operand(m_z, z);
            return *this;
        }

    private:
        float m_x = {0.0f}, m_y = {0.0f}, m_z = {0.0f}, m_w = {1.0f};

};  // class Vector

/**
 * VectorIntoMatrixInsertionOrder defines in which order the Vector components are inserted into the Matrix.
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
 * \note W-components are not considered.
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
 * \note W-components are not considered.
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
 * \param order       - the order, in which components of the Vector are inserted into the Matrix.
 * \param index       - the index of the row/column, in which the components of the Vector are inserted.
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
 * \note W-component of the result Vector is set to 1.
 * \param v1 - Vector 1.
 * \param v2 - Vector 2.
 * \return the cross product of two Vector.
 */
constexpr Vector crossProduct(const Vector& v1, const Vector& v2) noexcept
{
    // clang-format off
    return Vector{(v1.y() * v2.z()) - (v1.z() * v2.y()),
                  (v1.z() * v2.x()) - (v1.x() * v2.z()),
                  (v1.x() * v2.y()) - (v1.y() * v2.x())};  // clang-format on
}

/**
 * \brief Creates new normalized vector of the passed vector.
 *
 * \note W-component of the result Vector is set to 1.
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
 * \note W-components are not considered.
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
 * \note W-components are not considered.
 * \return true if two Vector have different coordinates, false otherwise.
 */
constexpr bool operator!=(const Vector& v1, const Vector& v2) noexcept
{
    return !(v1 == v2);
}

/**
 * \brief Adds two Vector.
 *
 * \note W-component of the result Vector is set to 1.
 * \return Vector, which is the result of addition of two Vector.
 */
constexpr Vector operator+(const Vector& v1, const Vector& v2) noexcept
{
    return Vector{v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

/**
 * \brief Adds number to a Vector.
 *
 * \note W-component of the result Vector is set to 1.
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
 * \note W-component of the result Vector is set to 1.
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
 * \note W-component of the result Vector is set to 1.
 * \return Vector, which is the result of subtraction of two Vector.
 */
constexpr Vector operator-(const Vector& v1, const Vector& v2) noexcept
{
    return Vector{v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

/**
 * \brief Subtracts the number from the Vector.
 *
 * \note W-component of the result Vector is set to 1.
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
 * \note W-component of the result Vector is set to 1.
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
 * \note W-component of the result Vector is set to 1.
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
 * \note W-component of the result Vector is set to 1.
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
 * \note W-components are not considered.
 * \return true if co-directed, false otherwise.
 */
inline bool isVectorsCodirected(const Vector& v1, const Vector& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), 0.0f);
}

/**
 * \brief Checks if two Vector are collinear.
 *
 * \note W-components are not considered.
 * \return true if collinear, false otherwise.
 */
inline bool isVectorsCollinear(const Vector& v1, const Vector& v2)
{
    return crossProduct(v1, v2).isZeroVector();
}

/**
 * \brief Checks if two Vector are oppositely directed (the angle between Vector is 180 degrees).
 *
 * \note W-components are not considered.
 * \return true if oppositely directed, false otherwise.
 */
inline bool isVectorsOppositelyDirected(const Vector& v1, const Vector& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), std::numbers::pi);
}

/**
 * \brief Checks if two Vector are orthogonal.
 *
 * \note W-components are not considered.
 * \return true if orthogonal, false otherwise.
 */
constexpr bool isVectorsOrthogonal(const Vector& v1, const Vector& v2) noexcept
{
    return helpers::isFloatsEqual(dotProduct(v1, v2), 0.0f);
}

}  // namespace ogls::mathCore

#endif
