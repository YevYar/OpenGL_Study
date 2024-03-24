#ifndef OGLS_MATHCORE_POINT_H
#define OGLS_MATHCORE_POINT_H

#include <iostream>

#include "helpers/floats.h"
#include "helpers/macros.h"
#include "mathCore/base.h"

namespace ogls::mathCore
{
/**
 * \brief Point represents point in 3D (2D) space.
 *
 * \param Type - a type of components of the Point.
 */
template<typename Type = float>
struct Point final
{
        static_assert(std::is_convertible_v<Type, float> || std::is_convertible_v<Type, int>
                        || std::is_convertible_v<Type, unsigned int> || std::is_convertible_v<Type, double>,
                      "A Point can be of the following types: float, double, int, unsigned int.");


    public:
        /**
         * \brief Constructs Point with all coordinates set to 0.
         */
        constexpr Point() noexcept = default;
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(Point)

        /**
         * \brief Constructs Point with passed X, Y and Z coordinates.
         */
        constexpr Point(Type xCoord, Type yCoord, Type zCoord = Type{0}) noexcept : x{xCoord}, y{yCoord}, z{zCoord}
        {
        }

        /**
         * \brief Constructs Point with all coordinates set to value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr explicit Point(Type generalCoordinate) noexcept :
            x{generalCoordinate}, y{generalCoordinate}, z{generalCoordinate}
        {
        }

        /**
         * \brief Returns true if this Point is zero-point (origin), false otherwise.
         *
         * \see isOrigin().
         */
        constexpr bool operator!() const noexcept
        {
            return !bool{*this};
        }

        /**
         * \brief Returns a boolean representation of the Point object.
         *
         * \see isOrigin().
         * \return false if the Point is zero-point (origin), true otherwise.
         */
        constexpr explicit operator bool() const noexcept
        {
            return !isOrigin();
        }

        /**
         * \brief Checks if the Point is zero-point (origin).
         *
         * \return true if the Point is zero-point (origin), false otherwise.
         */
        constexpr bool isOrigin() const noexcept
        {
            return x == Type{0} && y == Type{0} && z == Type{0};
        }

        /**
         * \brief Sets the X, Y, Z coordinates of the Point.
         *
         * \param x, y, z - values to set.
         */
        constexpr void setCoordinates(Type xCoord, Type yCoord, Type zCoord = Type{0}) noexcept
        {
            x = xCoord;
            y = yCoord;
            z = zCoord;
        }

        /**
         * \brief Sets the X, Y, Z coordinates of the Point to the value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr void setCoordinates(Type generalCoordinate) noexcept
        {
            x = generalCoordinate;
            y = generalCoordinate;
            z = generalCoordinate;
        }

        /**
         * \brief Returns a std::string representation of the Point object.
         */
        std::string toString() const
        {
            return std::format("Point(x={}, y={}, z={})", x, y, z);
        }

    public:
        Type x = Type{0}, y = Type{0}, z = Type{0};

};  // struct Point

/**
 * \brief Prints into the stream a std::string representation of the Point object.
 */
template<typename Type>
inline std::ostream& operator<<(std::ostream& out, const Point<Type>& p)
{
    out << p.toString();
    return out;
}

/**
 * \brief Checks equality of two Point.
 *
 * \return true if two Point have equal coordinates, false otherwise.
 */
template<typename Type>
constexpr bool operator==(const Point<Type>& p1, const Point<Type>& p2) noexcept
{
    using ogls::helpers::isEqual;


    return isEqual<Type>(p1.x, p2.x) && isEqual<Type>(p1.y, p2.y) && isEqual<Type>(p1.z, p2.z);
}

/**
 * \brief Checks if two Point are not equal.
 *
 * \return true if two Point have different coordinates, false otherwise.
 */
template<typename Type>
constexpr bool operator!=(const Point<Type>& p1, const Point<Type>& p2) noexcept
{
    return !(p1 == p2);
}

/**
 * \brief Calculates distance between two Point.
 *
 * \return distance between two Point.
 */
template<typename Type>
inline float distanceBetweenPoints(const Point<Type>& p1, const Point<Type>& p2) noexcept
{
    return p1 == p2 ? 0.0f : std::sqrtf(square(p2.x - p1.x) + square(p2.y - p1.y) + square(p2.z - p1.z));
}

}  // namespace ogls::mathCore

#endif
