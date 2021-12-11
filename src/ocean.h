#pragma once

#include "boundingbox.h"
#include "vec2.h"

#define ITERATIONS_NORMAL 48
#define ITERATIONS_RAYMARCH 12

struct alignas(16) Ocean
{
    BoundingBox bbox;
    
    float depth = 2.0f;
    float phase = 6.0f;
    float speed = 2.0f;
    float drag = 0.048;

};
    
bool Raymarch(const Ocean& ocean, RayHit& rayhit, const float time) noexcept;

bool Intersect(const Ocean& ocean, RayHit& rayhit) noexcept;

vec2 WaveDx(const vec2& position, const vec2& direction, const float speed, const float freq, const float timeshift) noexcept;

float Wave(const Ocean& ocean, const vec2& position, const uint8_t iterations, const float time) noexcept;

vec3 WaveNormal(const Ocean& ocean, const vec2& position, const float time, const float e = 0.01f) noexcept;