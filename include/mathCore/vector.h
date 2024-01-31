#ifndef OGLS_MATHCORE_VECTOR_H
#define OGLS_MATHCORE_VECTOR_H

#include <functional>

#include "exceptions.h"
#include "mathCore/point.h"

namespace ogls::mathCore
{
/**
 * \brief FunctorOnVectors checks if a provided functor can be applied to get new coordinate value based on two other
 * coordinates.
 */
template<typename Operator>
concept FunctorOnVectors = requires(Operator f) {
    // clang-format off
    {f(0.0f, 0.0f)} -> std::convertible_to<float>;  // clang-format on
};

/**
 * \brief IsVec3OrVec4 checks if the dimensionality of the Vector is 3 or 4.
 */
template<size_t Dimensionality>
concept IsVec3OrVec4 = (Dimensionality >= 3);

/**
 * \brief IsVec4 checks if the dimensionality of the Vector is 4.
 */
template<size_t Dimensionality>
concept IsVec4 = (Dimensionality == 4);

template<typename OtherIterator>
concept OtherVectorIterator = requires(OtherIterator obj) {
    // clang-format off
    {obj.equalToOtherIterator(0, nullptr)} -> std::same_as<bool>;  // clang-format on
};

template<size_t N>
class VectorImpl
{
};  // class VectorImpl

template<>
class VectorImpl<2>
{
    public:
        constexpr void setCoordinates(float x, float y, float z) noexcept
        {
            m_x = x;
            m_y = y;
        }

    public:
        float m_x = {0.0f}, m_y = {0.0f};

};  // class VectorImpl<2>

template<>
class VectorImpl<3>
{
    public:
        constexpr void setCoordinates(float x, float y, float z) noexcept
        {
            m_x = x;
            m_y = y;
            m_z = z;
        }

    public:
        float m_x = {0.0f}, m_y = {0.0f}, m_z = {0.0f};

};  // class VectorImpl<3>

template<>
class VectorImpl<4>
{
    public:
        constexpr void setCoordinates(float x, float y, float z) noexcept
        {
            m_x = x;
            m_y = y;
            m_z = z;
        }

    public:
        float m_x = {0.0f}, m_y = {0.0f}, m_z = {0.0f}, m_w = {1.0f};

};  // class VectorImpl<4>

template<size_t N>
class Vector;

/**
 * \brief Returns a new Vector whose coordinates are the results of applying the Operator to the corresponding
 * coordinates of v1 and v2.
 *
 * May throw an exception, if the Operator throws.
 *
 * \param N        - a number of components in the Vector.
 * \param Operator - a functor, which accepts two float values and returns float too.
 * \return a new Vector.
 */
template<size_t N, FunctorOnVectors Operator>
constexpr Vector<N> makeVector(const Vector<N>& v1, const Vector<N>& v2)
{
    const auto f = Operator{};

    if constexpr (N == 2)
    {
        return Vector<2>{f(v1.x(), v2.x()), f(v1.y(), v2.y())};
    }
    else
    {
        return Vector<N>{f(v1.x(), v2.x()), f(v1.y(), v2.y()), f(v1.z(), v2.z())};
    }
}

/**
 * \brief Returns a new Vector whose coordinates are the results of applying the Operator to the corresponding
 * coordinates of v1 and num.
 *
 * May throw an exception, if the Operator throws.
 *
 * \param N        - a number of components in the Vector.
 * \param Operator - a functor, which accepts two float values and returns float too.
 * \param v        - a Vector whose coordinates are used by the Operator.
 * \param num      - a number on which the coordinates of v1 are updated.
 * \return a new Vector.
 */
template<size_t N, FunctorOnVectors Operator>
constexpr Vector<N> makeVector(const Vector<N>& v, float num)
{
    const auto f = Operator{};

    if constexpr (N == 2)
    {
        return Vector<2>{f(v.x(), num), f(v.y(), num)};
    }
    else
    {
        return Vector<N>{f(v.x(), num), f(v.y(), num), f(v.z(), num)};
    }
}

/**
 * \brief Vector represents a vector in 2D or 3D orthonormal basis
 * whose components can be represented in Cartesian or Homogeneous coordinates.
 *
 * \param N - a number of components in the Vector. It can be in the range [2, 4]:<br>
 * 2 - 2D vector;<br>
 * 3 - 3D vector;<br>
 * 4 - 3D vector in [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates).
 * \note The W-component of the Vector<4> is usually set to 1.0 and isn't taken into account in calculations.
 * It can only be set explicitly using Vector(float, float, float, float) and setW(float).
 * However, operator==() takes into account W-component.
 */
template<size_t N>
class Vector
{
        static_assert(N >= 2 && N <= 4, "Number of components in the Vector must be in the range [2, 4].");


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
                struct Component final
                {
                    public:
                        /**
                         * \brief Allows copy assignment of the float value.
                         *
                         * Changes the referenced Vector component. Makes it possible to use algorithms like std::fill().
                         */
                        constexpr Component& operator=(ComponentType newValue) noexcept
                        requires IsNotConstType<ComponentType>
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
                        explicit operator std::string() const
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
                                    if constexpr (N >= 3)
                                    {
                                        component = 'Z';
                                        break;
                                    }
                                    else
                                    {
                                        [[fallthrough]];
                                    }
                                case 3:
                                    if constexpr (N == 4)
                                    {
                                        component = 'W';
                                        break;
                                    }
                                    else
                                    {
                                        [[fallthrough]];
                                    }
                                default:
                                    OGLS_ASSERT(false);
                                    return std::format("Vector{}::Iterator::Unknown Component", N);
                            }
                            return std::format("Vector{}::Iterator::{} = {}", N, component, getValue());
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
                        constexpr void setValue(ComponentType newValue) noexcept
                        requires IsNotConstType<ComponentType>
                        {
                            getValue() = newValue;
                        }

                    public:
                        /**
                         * \brief The index of the Vector component.
                         */
                        size_t         i     = {4};  // an invalid state
                        /**
                         * \brief The pointer to the value of the Component.
                         */
                        ComponentType* value = nullptr;

                };  // struct Component

                using VectorType = std::conditional_t<std::is_const_v<ComponentType>, const Vector<N>, Vector<N>>;

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
                constexpr explicit Iterator(VectorType* vec, bool isEndIterator = false) noexcept :
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
                    auto value = &m_vec->impl.m_x;
                    switch (m_i)
                    {
                        case 0:
                            value = &m_vec->impl.m_x;
                            break;
                        case 1:
                            value = &m_vec->impl.m_y;
                            break;
                        case 2:
                            if constexpr (N >= 3)
                            {
                                value = &m_vec->impl.m_z;
                                break;
                            }
                            else
                            {
                                [[fallthrough]];
                            }
                        case 3:
                            if constexpr (N == 4)
                            {
                                value = &m_vec->impl.m_w;
                                break;
                            }
                            else
                            {
                                [[fallthrough]];
                            }
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
                constexpr bool equalToOtherIterator(size_t index, const Vector<N>* vec) const noexcept
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

                    if (newIndex < N)
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
                size_t      m_i                    = {4};  // an invalid state
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
         * \brief Constructs Vector with passed Vector coordinates X and Y.
         */
        constexpr Vector(float x, float y) noexcept
        {
            impl.setCoordinates(x, y, 0.0f);
        }

        /**
         * \brief Constructs Vector with passed Vector coordinates X, Y and Z.
         */
        constexpr Vector(float x, float y, float z) noexcept
        requires IsVec3OrVec4<N>
        {
            impl.setCoordinates(x, y, z);
        }

        /**
         * \brief Constructs Vector with passed Vector coordinates X, Y and Z and W-component.
         *
         * \see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates).
         */
        constexpr Vector(float x, float y, float z, float w) noexcept
        requires IsVec4<N>
        {
            impl.setCoordinates(x, y, z);
            impl.m_w = w;
        }

        /**
         * \brief Constructs Vector from two Point.
         *
         * \param p1 - a start point of the Vector.
         * \param p2 - an end point of the Vector.
         */
        constexpr Vector(const Point& p1, const Point& p2) noexcept
        {
            impl.setCoordinates(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
        }

        /**
         * \brief Constructs Vector with coordinates set to the value of generalCoordinate.
         *
         * \param generalCoordinate - a value of Vector coordinates.
         */
        constexpr explicit Vector(float generalCoordinate) noexcept
        {
            impl.setCoordinates(generalCoordinate, generalCoordinate, generalCoordinate);
        }

        /**
         * \brief Constructs Vector<3>/Vector<4> from the passed Vector<2>.
         */
        constexpr explicit Vector(const Vector<2>& v) noexcept
        requires IsVec3OrVec4<N>
        {
            impl.setCoordinates(v.x(), v.y(), 0.0f);
        }

        /**
         * \brief Constructs Vector<4> from the passed Vector<3>.
         */
        constexpr explicit Vector(const Vector<3>& v) noexcept
        requires IsVec4<N>
        {
            impl.setCoordinates(v.x(), v.y(), v.z());
        }

        /**
         * \brief Constructs Vector from the passed Vector of the bigger dimensionality.
         */
        template<size_t VectorDimensionality>
        requires(VectorDimensionality > N)
        constexpr explicit Vector(const Vector<VectorDimensionality>& v) noexcept
        {
            if constexpr (N == 2)
            {
                impl.setCoordinates(v.x(), v.y(), 0);
            }
            else if constexpr (N == 3)
            {
                impl.setCoordinates(v.x(), v.y(), v.z());
            }
        }

        //------ SOME UNARY OPERATIONS

        /**
         * \brief Returns new Vector as the result of negation of this Vector.
         */
        constexpr Vector operator-() const noexcept
        {
            if constexpr (N == 2)
            {
                return Vector<2>{-impl.m_x, -impl.m_y};
            }
            else if constexpr (N == 3)
            {
                return Vector<3>{-impl.m_x, -impl.m_y, -impl.m_z};
            }
            else
            {
                return Vector<4>{-impl.m_x, -impl.m_y, -impl.m_z, impl.m_w};
            }
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
            return changeCoordinatesViaOperator<std::plus<float>>(v);
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
            return changeCoordinatesViaOperator<std::plus<float>>(num, num, num);
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
            return changeCoordinatesViaOperator<std::minus<float>>(v);
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
            return changeCoordinatesViaOperator<std::minus<float>>(num, num, num);
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
            return changeCoordinatesViaOperator<std::multiplies<float>>(num, num, num);
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

            return changeCoordinatesViaOperator<std::divides<float>>(num, num, num);
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
            if constexpr (N == 2)
            {
                return std::format("Vector2(x={}, y={} | length={})", impl.m_x, impl.m_y, length());
            }
            else if constexpr (N == 3)
            {
                return std::format("Vector3(x={}, y={}, z={} | length={})", impl.m_x, impl.m_y, impl.m_z, length());
            }
            else
            {
                return std::format("Vector4(x={}, y={}, z={}, w={} | length={})", impl.m_x, impl.m_y, impl.m_z,
                                   impl.m_w, length());
            }
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
            return N;
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
            if constexpr (N == 2)
            {
                return impl.m_x == 0.0f && impl.m_y == 0.0f;
            }
            else
            {
                return impl.m_x == 0.0f && impl.m_y == 0.0f && impl.m_z == 0.0f;
            }
        }

        /**
         * \brief Returns the length (magnitude) of the Vector.
         */
        constexpr float length() const noexcept
        {
            if constexpr (N == 2)
            {
                return impl.m_x == 0.0f && impl.m_y == 0.0f ? 0.0f : std::sqrtf(square(impl.m_x) + square(impl.m_y));
            }
            else
            {
                return impl.m_x == 0.0f && impl.m_y == 0.0f && impl.m_z == 0.0f
                         ? 0.0f
                         : std::sqrtf(square(impl.m_x) + square(impl.m_y) + square(impl.m_z));
            }
        }

        /**
         * \brief Sets the W-component of the Vector.
         *
         * \param w - value to set.
         */
        constexpr void setW(float w) noexcept
        requires IsVec4<N>
        {
            impl.m_w = w;
        }

        /**
         * \brief Sets the X coordinate of the Vector.
         *
         * \param x - value to set.
         */
        constexpr void setX(float x) noexcept
        {
            impl.m_x = x;
        }

        /**
         * \brief Sets the Y coordinate of the Vector.
         *
         * \param y - value to set.
         */
        constexpr void setY(float y) noexcept
        {
            impl.m_y = y;
        }

        /**
         * \brief Sets the Z coordinate of the Vector.
         *
         * \param z - value to set.
         */
        constexpr void setZ(float z) noexcept
        requires IsVec3OrVec4<N>
        {
            impl.m_z = z;
        }

        /**
         * \brief Sets the X and Y coordinates of the Vector.
         *
         * \param x, y - values to set.
         */
        constexpr void setCoordinates(float x, float y) noexcept
        {
            impl.setCoordinates(x, y, 0.0f);
        }

        /**
         * \brief Sets the X, Y and Z coordinates of the Vector.
         *
         * \param x, y, z - values to set.
         */
        constexpr void setCoordinates(float x, float y, float z) noexcept
        requires IsVec3OrVec4<N>
        {
            impl.setCoordinates(x, y, z);
        }

        /**
         * \brief Sets all coordinates of the Vector to the value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr void setCoordinates(float generalCoordinate) noexcept
        {
            impl.setCoordinates(generalCoordinate, generalCoordinate, generalCoordinate);
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
        requires IsVec4<N>
        {
            return impl.m_w;
        }

        /**
         * \brief Returns X coordinate of the Vector.
         */
        constexpr float x() const noexcept
        {
            return impl.m_x;
        }

        /**
         * \brief Returns Y coordinate of the Vector.
         */
        constexpr float y() const noexcept
        {
            return impl.m_y;
        }

        /**
         * \brief Returns Z coordinate of the Vector.
         */
        constexpr float z() const noexcept
        requires IsVec3OrVec4<N>
        {
            return impl.m_z;
        }

    private:
        /**
         * \brief Returns this Vector with changed coordinates in the result of applying of the Operator to all
         * coordinates of the Vector.
         *
         * May throw an exception, if the Operator throws.
         */
        template<FunctorOnVectors Operator>
        constexpr Vector& changeCoordinatesViaOperator(float x, float y, float z)
        {
            const auto f = Operator{};

            if constexpr (N == 2)
            {
                impl.setCoordinates(f(impl.m_x, x), f(impl.m_y, y), 0.0f);
            }
            else
            {
                impl.setCoordinates(f(impl.m_x, x), f(impl.m_y, y), f(impl.m_z, z));
            }

            return *this;
        }

        /**
         * \brief Returns this Vector with changed coordinates, which are the results of applying the
         * Operator to the corresponding coordinates of this Vector and v.
         *
         * May throw an exception, if the Operator throws.
         */
        template<FunctorOnVectors Operator>
        constexpr Vector& changeCoordinatesViaOperator(const Vector& v)
        {
            const auto f = Operator{};

            if constexpr (N == 2)
            {
                impl.setCoordinates(f(impl.m_x, v.impl.m_x), f(impl.m_y, v.impl.m_y), 0.0f);
            }
            else
            {
                impl.setCoordinates(f(impl.m_x, v.impl.m_x), f(impl.m_y, v.impl.m_y), f(impl.m_z, v.impl.m_z));
            }

            return *this;
        }

    private:
        VectorImpl<N> impl;

};  // class Vector

/**
 * \brief Vec2 represents a vector in 2D orthonormal basis.
 */
using Vec2 = Vector<2>;

/**
 * \brief Vec3 represents a vector in 3D orthonormal basis.
 */
using Vec3 = Vector<3>;

/**
 * \brief Vec4 represents a vector in 3D orthonormal basis with homogeneous coordinates.
 *
 * It has W-component.
 * The W-component of Vec4 is usually set to 1.0 and isn't taken into account in calculations.
 * It can only be set explicitly using Vec4(float, float, float, float) and setW(float).
 * However, operator==() takes into account W-component.
 *
 * \see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates).
 */
using Vec4 = Vector<4>;

//------ OPERATIONS ON VECTOR

/**
 * \brief Calculates a dot product of two Vector by their coordinates.
 *
 * \return the dot product of two Vector.
 */
template<size_t N>
constexpr float dotProduct(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    if constexpr (N == 2)
    {
        return (v1.x() * v2.x()) + (v1.y() * v2.y());
    }
    else
    {
        return (v1.x() * v2.x()) + (v1.y() * v2.y()) + (v1.z() * v2.z());
    }
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
template<size_t N>
inline float dotProduct(const Vector<N>& v1, const Vector<N>& v2, float angle, AngleUnit unit = AngleUnit::Degrees)
{
    return v1.length() * v2.length() * mathCore::cos(angle, unit);
}

/**
 * \brief Calculates the value of cosine between two Vector.
 *
 * \return the value of cosine between two Vector.
 */
template<size_t N>
constexpr float cosBetweenVectors(const Vector<N>& v1, const Vector<N>& v2) noexcept
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
template<size_t N>
inline float angleBetweenVectors(const Vector<N>& v1, const Vector<N>& v2)
{
    return std::acosf(mapValueToUnitRange(cosBetweenVectors(v1, v2)));
}

/**
 * \brief Calculates a cross product of two Vector.
 *
 * \param v1 - Vector 1.
 * \param v2 - Vector 2.
 * \return the cross product of two Vector.
 */
template<size_t N>
requires IsVec3OrVec4<N>
constexpr Vector<N> crossProduct(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    // clang-format off
    return Vector<N>{(v1.y() * v2.z()) - (v1.z() * v2.y()),
                     (v1.z() * v2.x()) - (v1.x() * v2.z()),
                     (v1.x() * v2.y()) - (v1.y() * v2.x())};  // clang-format on
}

/**
 * \brief Creates new normalized vector of the passed vector.
 *
 * \param v - the Vector to normalize.
 * \return normalized Vector of the Vector v or zero-vector if v is zero-vector.
 */
template<size_t N>
constexpr Vector<N> normalize(const Vector<N>& v) noexcept
{
    const auto length = v.length();
    return length == 0.0f ? v : makeVector<N, std::divides<float>>(v, length);
}

/**
 * \brief Checks equality of two Vector.
 *
 * \return true if two Vector have equal coordinates, false otherwise.
 */
template<size_t N>
constexpr bool operator==(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    using ogls::helpers::isFloatsEqual;


    if constexpr (N == 2)
    {
        return isFloatsEqual(v1.x(), v2.x()) && isFloatsEqual(v1.y(), v2.y());
    }
    else if constexpr (N == 3)
    {
        return isFloatsEqual(v1.x(), v2.x()) && isFloatsEqual(v1.y(), v2.y()) && isFloatsEqual(v1.z(), v2.z());
    }
    else
    {
        return isFloatsEqual(v1.x(), v2.x()) && isFloatsEqual(v1.y(), v2.y()) && isFloatsEqual(v1.z(), v2.z())
               && isFloatsEqual(v1.w(), v2.w());
    }
}

/**
 * \brief Checks if two Vector are not equal.
 *
 * \return true if two Vector have different coordinates, false otherwise.
 */
template<size_t N>
constexpr bool operator!=(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    return !(v1 == v2);
}

/**
 * \brief Adds two Vector.
 *
 * \return Vector, which is the result of addition of two Vector.
 */
template<size_t N>
constexpr Vector<N> operator+(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    return makeVector<N, std::plus<float>>(v1, v2);
}

/**
 * \brief Adds number to a Vector.
 *
 * \param num - a number to add to every coordinate of the Vector.
 * \param v   - a source Vector.
 * \return new Vector, which is a result of addition of the number to every coordinate of Vector v.
 */
template<size_t N>
constexpr Vector<N> operator+(float num, const Vector<N>& v) noexcept
{
    return makeVector<N, std::plus<float>>(v, num);
}

/**
 * \brief Adds number to a Vector.
 *
 * \param v   - a source Vector.
 * \param num - a number to add to every coordinate of the Vector.
 * \return new Vector, which is a result of addition of the number to every coordinate of Vector v.
 */
template<size_t N>
constexpr Vector<N> operator+(const Vector<N>& v, float num) noexcept
{
    return num + v;
}

/**
 * \brief Subtracts two Vector.
 *
 * \return Vector, which is the result of subtraction of two Vector.
 */
template<size_t N>
constexpr Vector<N> operator-(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    return makeVector<N, std::minus<float>>(v1, v2);
}

/**
 * \brief Subtracts the number from the Vector.
 *
 * \param v   - a source Vector.
 * \param num - a number to subtract from every coordinate of the Vector.
 * \return new Vector, which is a result of subtraction of the number from every coordinate of Vector v.
 */
template<size_t N>
constexpr Vector<N> operator-(const Vector<N>& v, float num) noexcept
{
    return makeVector<N, std::minus<float>>(v, num);
}

/**
 * \brief Multiplies all coordinates of the Vector by number.
 *
 * \param num - a number to multiply coordinates of the Vector by.
 * \param v   - a source Vector.
 * \return new Vector, which is a result of multiplication of every coordinate of Vector v by number.
 */
template<size_t N>
constexpr Vector<N> operator*(float num, const Vector<N>& v) noexcept
{
    return makeVector<N, std::multiplies<float>>(v, num);
}

/**
 * \brief Multiplies all coordinates of the Vector by number.
 *
 * \param v   - a source Vector.
 * \param num - a number to multiply coordinates of the Vector by.
 * \return new Vector, which is a result of multiplication of every coordinate of Vector v by number.
 */
template<size_t N>
constexpr Vector<N> operator*(const Vector<N>& v, float num) noexcept
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
template<size_t N>
constexpr Vector<N> operator/(const Vector<N>& v, float num)
{
    if (num == 0.0f)
    {
        throw ogls::exceptions::DivisionByZeroException{std::format("{} / {}", std::string{v}, num)};
    }

    return makeVector<N, std::divides<float>>(v, num);
}

//------ CHECKERS OF VECTOR PROPERTIES

/**
 * \brief Checks if two Vector are co-directed.
 *
 * \return true if co-directed, false otherwise.
 */
template<size_t N>
inline bool isVectorsCodirected(const Vector<N>& v1, const Vector<N>& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), 0.0f);
}

/**
 * \brief Checks if two Vector are collinear.
 *
 * \return true if collinear, false otherwise.
 */
template<size_t N>
constexpr bool isVectorsCollinear(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    if constexpr (N == 2)
    {
        return crossProduct(Vector<3>{v1}, Vector<3>{v2}).isZeroVector();
    }
    else
    {
        return crossProduct(v1, v2).isZeroVector();
    }
}

/**
 * \brief Checks if two Vector are oppositely directed (the angle between Vector is 180 degrees).
 *
 * \return true if oppositely directed, false otherwise.
 */
template<size_t N>
inline bool isVectorsOppositelyDirected(const Vector<N>& v1, const Vector<N>& v2)
{
    return helpers::isFloatsEqual(angleBetweenVectors(v1, v2), std::numbers::pi);
}

/**
 * \brief Checks if two Vector are orthogonal.
 *
 * \return true if orthogonal, false otherwise.
 */
template<size_t N>
constexpr bool isVectorsOrthogonal(const Vector<N>& v1, const Vector<N>& v2) noexcept
{
    return helpers::isFloatsEqual(dotProduct(v1, v2), 0.0f);
}

}  // namespace ogls::mathCore

#endif
