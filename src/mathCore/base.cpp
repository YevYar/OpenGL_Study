#include "base.h"

namespace ogls::mathCore
{
float mapAngleToCircleRange(float angle, AngleUnit unit) noexcept
{
    angle = helpers::absolute(angle);

    if (unit == AngleUnit::Degrees && angle >= 360.0f)
    {
        return std::fmod(angle, 360.0f);
    }
    if (unit == AngleUnit::Radians && angle >= std::numbers::pi * 2.0f)
    {
        return std::fmod(angle, std::numbers::pi * 2.0f);
    }
    return angle;
}

float cos(float angle, AngleUnit unit)
{
    const auto mappedAngle = mapAngleToCircleRange(angle, unit);

    // cos(0) = 1.0
    if (isAngle0(mappedAngle))
    {
        return 1.0f;
    }
    // cos(90) = cos(-90) = cos(270) = cos(-270) = 0
    if (isAngle90(mappedAngle, unit) || isAngle270(mappedAngle, unit))
    {
        return 0.0f;
    }
    // cos(180) = cos(-180) = -1
    if (isAngle180(mappedAngle, unit))
    {
        return -1.0f;
    }

    return std::cosf(unit == AngleUnit::Degrees ? degreesIntoRadians(angle) : angle);
}

std::optional<float> cot(float angle, AngleUnit unit)
{
    const auto tg = tan(angle, unit);

    if (tg)
    {
        return *tg == 0.0f ? std::nullopt : std::optional(1.0f / *tg);
    }

    return 0.0f;
}

float sin(float angle, AngleUnit unit)
{
    const auto sign        = float{angle >= 0.0f ? 1.0f : -1.0f};
    const auto mappedAngle = mapAngleToCircleRange(angle, unit);

    // sin(0) = sin(180) = sin(-180) = 0
    if (isAngle0(mappedAngle) || isAngle180(mappedAngle, unit))
    {
        return 0.0f;
    }
    // sin(90) = 1, sin(-90) = -1
    if (isAngle90(mappedAngle, unit))
    {
        return sign * 1.0f;
    }
    // sin(270) = -1, sin(-270) = 1
    if (isAngle270(mappedAngle, unit))
    {
        return sign * (-1.0f);
    }

    return std::sinf(unit == AngleUnit::Degrees ? degreesIntoRadians(angle) : angle);
}

std::optional<float> tan(float angle, AngleUnit unit)
{
    const auto mappedAngle = mapAngleToCircleRange(angle, unit);

    // tan(0) = tan(180) = tan(-180) = 0.0
    if (isAngle0(mappedAngle) || isAngle180(mappedAngle, unit))
    {
        return 0.0f;
    }
    // tan(90) = tan(-90) = tan(270) = tan(-270) = NaN
    if (isAngle90(mappedAngle, unit) || isAngle270(mappedAngle, unit))
    {
        return std::nullopt;
    }

    return std::tanf(unit == AngleUnit::Degrees ? degreesIntoRadians(angle) : angle);
}

}  // namespace ogls::mathCore
