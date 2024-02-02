#ifndef OGLS_HELPERS_FLOATS_H
#define OGLS_HELPERS_FLOATS_H

#include <cmath>

namespace ogls::helpers
{
/**
 * \brief The precision of float-type calculations.
 */
constexpr inline auto OGLS_FLOAT_EPSILON = float{1.0E-6};

/**
 * \brief Returns the absolute value of x: |x|.
 *
 * Constexpr replacement of std::fabs().
 *
 * \param x - a value to get absolute value of.
 * \return the absolute value of x.
 */
template<typename Type>
constexpr Type absolute(Type x) noexcept
{
    return x >= Type{0} ? x : -x;
}

/**
 * \brief Checks if provided float values a and b are equal with precision.
 *
 * \param precision - threshold value (an epsilon), which represents the acceptable difference
 * between two floating-point numbers to consider them equal.
 * \return true if a == b, false otherwise.
 */
constexpr bool isFloatsEqual(float a, float b, float precision = OGLS_FLOAT_EPSILON) noexcept
{
    return absolute(a - b) < precision;
}

/**
 * \brief Checks if provided float values a and b are not equal with precision.
 *
 * \param precision - threshold value (an epsilon), which represents the acceptable difference
 * between two floating-point numbers to consider them equal.
 * \return true if a != b, false otherwise.
 */
constexpr bool isFloatsNotEqual(float a, float b, float precision = OGLS_FLOAT_EPSILON) noexcept
{
    return !isFloatsEqual(a, b, precision);
}

}  // namespace ogls::helpers

#endif
