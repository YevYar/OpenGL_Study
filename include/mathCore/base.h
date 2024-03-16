#ifndef OGLS_MATHCORE_BASE_H
#define OGLS_MATHCORE_BASE_H

#include <cmath>
#include <format>
#include <numbers>
#include <optional>

#include "floats.h"

/**
 * \namespace ogls::mathCore
 * \brief mathCore namespace contains types and functions, which implement all necessary stuff for 3D-programming math.
 */
namespace ogls::mathCore
{
/**
 * \brief The precision of float-type calculations in trigonometric functions.
 */
constexpr inline auto OGLS_FLOAT_ANGLE_EPSILON = float{1.0E-6};

/**
 * \brief Multipliable is a concept, which specifies the types, which support operator*.
 *
 * \param Type - a type to check constraints of.
 */
template<typename Type>
concept Multipliable = requires(Type a, Type b) { noexcept(a * b); };

/**
 * \brief AngleUnit specifies measurement unit of the angle in calculations.
 */
enum class AngleUnit
{
    Degrees,
    Radians
};

//------ GENERAL FUNCTIONS

/**
 * \brief Returns string representation of the value of type AngleUnit.
 *
 * \param unit - a value to get string representation of.
 * \return string representation of the value of type AngleUnit.
 */
inline std::string angleUnitToString(AngleUnit unit)
{
    return unit == AngleUnit::Degrees ? "degrees" : "radians";
}

/**
 * \brief Calculates the cube of a number.
 *
 * \param Type - a type, which supports operator*.
 * \param x    - a number to calculate the cube of.
 * \return the cube of a number.
 */
template<Multipliable Type>
constexpr Type cube(Type x) noexcept
{
    return x * x * x;
}

/**
 * \brief Determines if the given floating point number is normal, i.e. is neither subnormal, infinite, nor NaN.
 *
 * The same as std::isnormal(), except that zero is considered normal.
 *
 * \param num - a value to check.
 * \return true, if the number is not subnormal, not infinite and not NaN, false otherwise.
 */
inline bool isNormal(float num) noexcept
{
    return num == 0.0f ? true : std::isnormal(num);
}

/**
 * \brief Maps the value to the range [-1, 1].
 *
 * If the value is outside the range, it is mapped to -1 or 1 depending on the sign.
 * Otherwise unchanged value is returned.
 *
 * \param value - a value to map.
 * \return the value mapped to the range [-1, 1].
 */
constexpr float mapValueToUnitRange(float value) noexcept
{
    if (value >= -1.0f && value <= 1.0f)
    {
        return value;
    }
    return value > 1.0f ? 1.0f : -1.0f;
}

/**
 * \brief Returns the sign of the value.
 *
 * \return -1, 0 or 1.
 */
template<typename Type>
constexpr int sign(Type value) noexcept
{
    return (Type{0} < value) - (value < Type{0});
}

/**
 * \brief Calculates the square of a number.
 *
 * \param Type - a type, which supports operator*.
 * \param x    - a number to calculate the square of.
 * \return the square of a number.
 */
template<Multipliable Type>
constexpr Type square(Type x) noexcept
{
    return x * x;
}

//------ ANGLE HANDLERS

/**
 * \brief Converts degrees into radians.
 *
 * \param degrees - a value of degrees to convert.
 * \return angle value in radians.
 */
constexpr float degreesIntoRadians(float degrees) noexcept
{
    constexpr auto k = std::numbers::pi / 180.0f;
    return degrees * k;
}

/**
 * \brief Checks if the angle is equal to 0 degrees.
 *
 * \param angle - a value of the angle.
 * \return true if the angle is equal to 0 degrees, false otherwise.
 */
constexpr bool isAngle0(float angle) noexcept
{
    return angle == 0.0f;
}

/**
 * \brief Checks if the angle is equal to 90 degrees.
 *
 * The value of the angle is compared with 90 degrees (or respective angle in radians) with precision
 * OGLS_FLOAT_ANGLE_EPSILON.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return true if the angle is equal to 90 degrees (or respective angle in radians), false otherwise.
 */
constexpr bool isAngle90(float angle, AngleUnit unit = AngleUnit::Degrees) noexcept
{
    constexpr auto radiansFor90Deg = float{std::numbers::pi / 2.0f};
    return helpers::isFloatsEqual(angle, unit == AngleUnit::Degrees ? 90.0f : radiansFor90Deg,
                                  OGLS_FLOAT_ANGLE_EPSILON);
}

/**
 * \brief Checks if the angle is equal to 180 degrees.
 *
 * The value of the angle is compared with 180 degrees (or respective angle in radians) with precision
 * OGLS_FLOAT_ANGLE_EPSILON.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return true if the angle is equal to 180 degrees (or respective angle in radians), false otherwise.
 */
constexpr bool isAngle180(float angle, AngleUnit unit = AngleUnit::Degrees) noexcept
{
    return helpers::isFloatsEqual(angle, unit == AngleUnit::Degrees ? 180.0f : std::numbers::pi,
                                  OGLS_FLOAT_ANGLE_EPSILON);
}

/**
 * \brief Checks if the angle is equal to 270 degrees.
 *
 * The value of the angle is compared with 270 degrees (or respective angle in radians) with precision
 * OGLS_FLOAT_ANGLE_EPSILON.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return true if the angle is equal to 270 degrees (or respective angle in radians), false otherwise.
 */
constexpr bool isAngle270(float angle, AngleUnit unit = AngleUnit::Degrees) noexcept
{
    constexpr auto radiansFor270Deg = float{3.0f * std::numbers::pi / 2.0f};
    return helpers::isFloatsEqual(angle, unit == AngleUnit::Degrees ? 270.0f : radiansFor270Deg,
                                  OGLS_FLOAT_ANGLE_EPSILON);
}

/**
 * \brief Maps the value to the range [0, 360] degrees.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return the angle mapped to the range [0, 360] degrees.
 */
float mapAngleToCircleRange(float angle, AngleUnit unit = AngleUnit::Degrees) noexcept;

/**
 * \brief Converts radians into degrees.
 *
 * \param radians - a value of radians to convert.
 * \return angle value in degrees.
 */
constexpr float radiansIntoDegrees(float radians) noexcept
{
    constexpr auto k = 180.0f / std::numbers::pi;
    return radians * k;
}

//------ TRIGONOMETRIC FUNCTIONS

/**
 * \brief Calculates cosine of the angle.
 *
 * The angle is manually checked to ensure that for the angles 0, 90, 180 and 270 degrees the exact cos value
 * (1, 0, -1, 0 respectively) is returned, despite the fact that such degrees may be represented in float format with
 * some error (like 180.00000001).
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return cosine of the angle.
 */
float cos(float angle, AngleUnit unit = AngleUnit::Degrees);

/**
 * \brief Calculates cotangent of the angle.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return cotangent of the angle or std::nullopt, if cotangent does not exist for the angle.
 */
std::optional<float> cot(float angle, AngleUnit unit = AngleUnit::Degrees);

/**
 * \brief Calculates sine of the angle.
 *
 * The angle is manually checked to ensure that for the angles 0, 90, 180 and 270 degrees the exact sin value
 * (0, 1, 0, -1 respectively) is returned, despite the fact that such degrees may be represented in float format with
 * some error (like 180.00000001).
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return sine of the angle.
 */
float sin(float angle, AngleUnit unit = AngleUnit::Degrees);

/**
 * \brief Calculates tangent of the angle.
 *
 * \param angle - a value of the angle.
 * \param unit  - measurement unit of the angle.
 * \return tangent of the angle or std::nullopt, if tangent does not exist for the angle.
 */
std::optional<float> tan(float angle, AngleUnit unit = AngleUnit::Degrees);

}  // namespace ogls::mathCore

#endif
