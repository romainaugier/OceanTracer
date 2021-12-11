#pragma once

#include "vec3.h"

struct Sun
{
    vec3 direction = normalize(vec3(200.0f, 100.0f, 10.0f));
    vec3 color = vec3(2.0f, 2.0f, 2.0f);
};

struct Sky
{
    Sun sun;

    vec3 color1 = vec3(0.5f, 0.7f, 1.0f);
    vec3 color2 = vec3(0.1f, 0.2f, 0.3f);
};

FORCEINLINE vec3 SampleSun(const vec3& direction, const Sun& sun) noexcept
{
    const float sunDot = dot(direction, sun.direction);
    const float intensity = sunDot > 0.0f ? maths::pow(maths::clamp(sunDot), 1000.0f) : maths::constants::zero;

    return sun.color * intensity;
}

FORCEINLINE vec3 SampleSky(const vec3& direction, const Sky& sky) noexcept
{
    return lerp(sky.color1, sky.color2, maths::pow(maths::clamp(direction.y), 0.75f)) + SampleSun(direction, sky.sun);
}