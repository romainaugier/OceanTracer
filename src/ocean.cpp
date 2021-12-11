#include "ocean.h"

bool Raymarch(const Ocean& ocean, RayHit& rayhit, const float time) noexcept
{
    vec3 position = rayhit.hit.pos;
    float h = 0.0f;

    for(uint16_t i = 0; i < 300; i++)
    {
        h = Wave(ocean, vec2(position.x, position.z) * 0.1f, ITERATIONS_RAYMARCH, time) * ocean.depth - ocean.depth;
        if(h + 0.01f > position.y)
        {
            rayhit.hit.pos = position;
            rayhit.ray.t = dist2(rayhit.ray.origin, position);
            return true;
        }    
        position += rayhit.ray.direction * (position.y - h);
    }

    return false;
}

bool Intersect(const Ocean& ocean, RayHit& rayhit) noexcept
{
    return Slabs(ocean.bbox, rayhit);
}

vec2 WaveDx(const vec2& position, const vec2& direction, const float speed, const float freq, const float timeshift) noexcept
{
    const float x = dot(direction, position) * freq + timeshift * speed;
    const float wave = maths::exp(maths::sin(x) - 1.0f);
    const float dx = wave * maths::cos(x);
    return vec2(wave, -dx);
}

float Wave(const Ocean& ocean, const vec2& position, const uint8_t iterations, const float time) noexcept
{
    vec2 pos = position;
    float iter = 0.0f;
    float phase = ocean.phase;
    float speed = ocean.speed;
    float weight = 1.0f;
    float w = 0.0f;
    float ws = 0.0f;

    for(uint8_t i = 0; i < iterations; i++)
    {
        const vec2 p = vec2(sin(iter), cos(iter));
        const vec2 res = WaveDx(pos, p, speed, phase, time);
        pos += normalize(p) * res.y * weight * ocean.drag;
        w += res.x * weight;
        iter += 12.0f;
        ws += weight;
        weight = maths::lerp(weight, 0.0f, 0.2f);
        phase *= 1.18f;
        speed *= 1.07f;
    }

    return w / ws;
}

vec3 WaveNormal(const Ocean& ocean, const vec2& position, const float time, const float e) noexcept
{
    const vec2 ex = vec2(e, 0.0f);
    const float H = Wave(ocean, position * 0.1f, ITERATIONS_NORMAL, time) * ocean.depth - ocean.depth;
    const vec3 a = vec3(position.x, H, position.y);
    
    return normalize(cross(a - vec3(position.x - e, Wave(ocean, position * 0.1f - ex, ITERATIONS_NORMAL, time) * ocean.depth - ocean.depth, position.y),
                           a - vec3(position.x, Wave(ocean, position * 0.1f + vec2(ex.y, ex.x) * 0.1f, ITERATIONS_NORMAL, time) * ocean.depth - ocean.depth, position.y + e)));
}