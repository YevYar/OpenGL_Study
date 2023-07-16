#ifndef OGLS_MATHCORE_POINT_H
#define OGLS_MATHCORE_POINT_H

#include "helpers/floats.h"
#include "helpers/macros.h"
#include "mathCore/base.h"

namespace ogls::mathCore
{
/**
 * \brief Point represents point in 3D (2D) space.
 */
struct Point
{
    public:
        /**
         * \brief Constructs Point with all coordinates set to 0.
         */
        constexpr Point() noexcept = default;
        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(Point)

        /**
         * \brief Constructs Point with passed X, Y and Z coordinates.
         */
        constexpr Point(float xCoord, float yCoord, float zCoord = {0.0f}) noexcept : x{xCoord}, y{yCoord}, z{zCoord}
        {
        }

        /**
         * \brief Constructs Point with all coordinates set to value of generalCoordinate.
         *
         * \param generalCoordinate - value of X, Y and Z coordinates.
         */
        constexpr explicit Point(float generalCoordinate) noexcept :
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
         * \brief Returns a std::string representation of the Point object.
         */
        explicit operator std::string() const
        {
            return std::format("Point(x={}, y={}, z={})", x, y, z);
        }

        /**
         * \brief Checks if the Point is zero-point (origin).
         *
         * \return true if the Point is zero-point (origin), false otherwise.
         */
        constexpr bool isOrigin() const noexcept
        {
            return x == 0.0f && y == 0.0f && z == 0.0f;
        }

        /**
         * \brief Sets the X, Y, Z coordinates of the Point.
         *
         * \param x, y, z - values to set.
         */
        constexpr void setCoordinates(float xCoord, float yCoord, float zCoord = {0.0f}) noexcept
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
        constexpr void setCoordinates(float generalCoordinate) noexcept
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
            return static_cast<std::string>(*this);
        }

    public:
        float x = {0.0f}, y = {0.0f}, z = {0.0f};

};  // struct Point

/**
 * \brief Checks equality of two Point.
 *
 * \return true if two Point have equal coordinates, false otherwise.
 */
constexpr bool operator==(const Point& p1, const Point& p2) noexcept
{
    using ogls::helpers::isFloatsEqual;


    return isFloatsEqual(p1.x, p2.x) && isFloatsEqual(p1.y, p2.y) && isFloatsEqual(p1.z, p2.z);
}

/**
 * \brief Checks if two Point are not equal.
 *
 * \return true if two Point have different coordinates, false otherwise.
 */
constexpr bool operator!=(const Point& p1, const Point& p2) noexcept
{
    return !(p1 == p2);
}

/**
 * \brief Calculates distance between two Point.
 *
 * \return distance between two Point.
 */
inline float distanceBetweenPoints(const Point& p1, const Point& p2) noexcept
{
    return p1 == p2 ? 0.0f : std::sqrtf(square(p2.x - p1.x) + square(p2.y - p1.y) + square(p2.z - p1.z));
}

}  // namespace ogls::mathCore

#endif
