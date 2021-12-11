#pragma once

#include "bluenoise.h"
#include "vec3.h"

// Wang-Hash pseudo random number generators

FORCEINLINE int WangHash(int seed) noexcept
{
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 0x27d4eb2du;
    seed = seed ^ (seed >> 15u);
    return 1u + seed;
}


FORCEINLINE void WangHash4(int* seed) noexcept
{
    for (int i = 0; i < 4; i++)
    {
        seed[i] = (seed[i] ^ 61u) ^ (seed[i] >> 16u);
        seed[i] *= 9u;
        seed[i] = seed[i] ^ (seed[i] >> 4u);
        seed[i] *= 0x27d4eb2du;
        seed[i] = seed[i] ^ (seed[i] >> 15u);
        seed[i] += 1u;
    }
}


FORCEINLINE void WangHash8(int* seed) noexcept
{
    for (int i = 0; i < 8; i++)
    {
        seed[i] = (seed[i] ^ 61u) ^ (seed[i] >> 16u);
        seed[i] *= 9u;
        seed[i] = seed[i] ^ (seed[i] >> 4u);
        seed[i] *= 0x27d4eb2du;
        seed[i] = seed[i] ^ (seed[i] >> 15u);
        seed[i] += 1u;
    }
}


FORCEINLINE int XorShift32(int state) noexcept
{
    int x = state;
    x ^= x << 13u;
    x ^= x >> 17u;
    x ^= x << 5u;
    return x;
}


FORCEINLINE void XorShift324(int* state) noexcept
{
    for (int i = 0; i < 4; i++)
    {
        int x = state[i];
        x ^= x << 13u;
        x ^= x >> 17u;
        x ^= x << 5u;
        state[i] = x;
    }
}


FORCEINLINE void XorShift328(int* state) noexcept
{
    for (int i = 0; i < 8; i++)
    {
        int x = state[i];
        x ^= x << 13u;
        x ^= x >> 17u;
        x ^= x << 5u;
        state[i] = x;
    }
}


inline float WangHashSampler(int state) noexcept
{
    constexpr unsigned int floatAddr = 0x2f800004u;
    auto toFloat = float();
    memcpy(&toFloat, &floatAddr, 4);

    state = WangHash(state);
    int x = XorShift32(state);
    state = x;
    return static_cast<float>(x) * toFloat + 0.5f;
}


inline float* WangHashSampler4(int* state) noexcept
{
    constexpr unsigned int tofloat = 0x2f800004u;
    WangHash4(state);
    XorShift324(state);

    float randoms[4];
    
    for (int i = 0; i < 4; i++)
    {
        randoms[i] = static_cast<float>(state[i]) * reinterpret_cast<const float&>(tofloat) + 0.5f;
    }

    return randoms;
}


inline void WangHashSampler8(float* randoms, int* state) noexcept
{
    constexpr unsigned int floatAddr = 0x2f800004u;
    auto toFloat = float();
    memcpy(&toFloat, &floatAddr, 4);

    WangHash4(state);
    XorShift328(state);

    for (int i = 0; i < 8; i++)
    {
        randoms[i] = static_cast<float>(state[i]) * toFloat + 0.5f;
    }
}


// Pseudo random generator, not that slow (~20 cycles per generation)

inline float PseudoRandomSamplerSlow() noexcept
{
    constexpr unsigned int floatAddr = 0x2f800004u;
    auto toFloat = float();
    memcpy(&toFloat, &floatAddr, 4);

    unsigned long x = 123456789, y = 362436069, z = 521288629;

    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return static_cast<float>(z) * toFloat + 0.5f;
}

// The following two random float generation functions are from
// https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/

inline float PcgSampler(unsigned int state) noexcept
{
    constexpr unsigned int floatAddr = 0x2f800004u;
    auto toFloat = float();
    memcpy(&toFloat, &floatAddr, 4);

    const unsigned int state2 = state * 747796405u + 2891336453u;
    const unsigned int word = ((state2 >> ((state2 >> 28u) + 4u)) ^ state2) * 277803737u;
    state = (word >> 22u) ^ word;

    return static_cast<float>(state) * toFloat;
}


inline void PcgSampler8(float* randoms, unsigned int* state) noexcept
{
    __m256i states = _mm256_set1_epi32(94853);
    const __m256i constant1 = _mm256_set1_epi32(747796405u);
    const __m256i constant2 = _mm256_set1_epi32(2891336453u);
    const __m256i constant3 = _mm256_set1_epi32(277803737u);

    states = _mm256_add_epi32(_mm256_mul_epi32(states, constant1), constant2);

    const __m256i word = _mm256_mul_epi32(
        _mm256_xor_si256(
            _mm256_srav_epi32(states,
                _mm256_add_epi32(
                    _mm256_srav_epi32(states,
                        _mm256_set1_epi32(28u)),
                    _mm256_set1_epi32(4u))),
            states),
        constant3);

    states = _mm256_xor_si256(
        _mm256_srav_epi32(word,
            _mm256_set1_epi32(22u)),
        word);

    constexpr unsigned int floatAddr = 0x2f800004u;
    auto toFloat = float();
    memcpy(&toFloat, &floatAddr, 4);

    const __m256 tofloat2 = _mm256_set1_ps(toFloat);
    __m256 floats = _mm256_mul_ps(_mm256_cvtepi32_ps(states), tofloat2);
    floats = _mm256_add_ps(floats, _mm256_set1_ps(0.5f));
    _mm256_storeu_ps(randoms, floats);
}


// Blue noise sampling functions

// Eric Heitz optimised blue noise sampling function https://eheitzresearch.wordpress.com/762-2/

inline uint32_t* LoadBlueNoise()
{
    uint32_t* data = new uint32_t[65536 * 5];

    const uint8_t* data8 = sobol_256spp_256d;
    for(uint32_t i = 0; i < 65536; i++) data[i] = data8[i];

    data8 = scramblingTile;
    for(uint32_t i = 0; i < (128 * 128 * 8); i++) data[i + 65536] = data8[i];

    data8 = rankingTile;
    for(uint32_t i = 0; i < (128 * 128 * 8); i++) data[i + 3 * 65536] = data8[i];

    return data;
}

inline float BlueNoiseSamplerSpp(const uint32_t* blueNoise, uint32_t x, uint32_t y, uint32_t sampleIndex, uint32_t sampleDimension) noexcept
{
	// wrap arguments
	x = x & 127;
	y = y & 127;
	sampleIndex = sampleIndex & 255;
	sampleDimension = sampleDimension & 255;

	// xor index based on optimized ranking
	const uint32_t rankedSampleIndex = sampleIndex ^ blueNoise[sampleDimension + (x + y * 128) * 8 + 65536 * 3];

	// fetch value in sequence
	uint32_t value = blueNoise[sampleDimension + rankedSampleIndex * 256];

	// If the dimension is optimized, xor sequence value based on optimized scrambling
	value = value ^ blueNoise[(sampleDimension%8) + (x + y * 128) * 8 + 65536];

	// convert to float and return
	return (0.5f + value) / 256.0f;
}


// Sampling functions

// Classic cosine hemisphere sampling
inline vec3 SampleHemisphere(const vec3& hit_normal, const float rx, const float ry) noexcept
{
    float signZ = (hit_normal.z >= 0.0f) ? 1.0f : -1.0f;
    float a = -1.0f / (signZ + hit_normal.z);
    float b = hit_normal.x * hit_normal.y * a;
    vec3 b1 = vec3(1.0f + signZ * hit_normal.x * hit_normal.x * a, signZ * b, -signZ * hit_normal.x);
    vec3 b2 = vec3(b, signZ + hit_normal.y * hit_normal.y * a, -hit_normal.y);

    float phi = 2.0f * maths::constants::pi * rx;
    float cosTheta = maths::sqrt(ry);
    float sinTheta = maths::sqrt(1.0f - ry);

    return normalize(((b1 * maths::cos(phi) + b2 * maths::sin(phi)) * cosTheta + hit_normal * sinTheta));
}


// Not very safe
inline vec3 SampleHemisphereUnsafe(const vec3& hit_normal, const float rx, const float ry) noexcept
{
    float a = 1.0f - 2.0f * rx;
    float b = maths::sqrt(1.0f - a * a);
    float phi = 2.0f * maths::constants::pi * ry;

    return vec3(hit_normal.x + b * maths::cos(phi), hit_normal.y + b * maths::sin(phi), hit_normal.z + a);
}