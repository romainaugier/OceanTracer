#pragma once

// Very simple header only maths library
// Float only as I just use floats in the renderer

#include "decl.h"
#include <immintrin.h>
#include <limits>
#include <cstring>

#ifdef __GNUC__
#include <cmath>
#endif

namespace maths
{

    // Constants
    namespace constants 
    {
        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float pi_over_two = 1.57079632679489661923f;
        static constexpr float pi_over_four = 0.785398163397448309616f;
        static constexpr float one_over_pi = 0.318309886183790671538f;
        static constexpr float two_over_pi = 0.636619772367581343076;

        static constexpr float inf = std::numeric_limits<float>::infinity();
        static constexpr float neginf = -std::numeric_limits<float>::infinity();

        static constexpr float sqrt2 = 1.41421356237309504880f;
        static constexpr float one_over_sqrt2 = 0.707106781186547524401f;

        static constexpr float e = 2.71828182845904523536f;

        static constexpr float log2e = 1.44269504088896340736f;
        static constexpr float log10e = 0.434294481903251827651f;
        static constexpr float ln2 = 0.693147180559945309417f;
        static constexpr float ln10 = 2.30258509299404568402f;

        static constexpr float zero = 0.0f;
        static constexpr float one = 1.0f;
        static constexpr float min_float = std::numeric_limits<float>::lowest();
		static constexpr float max_float = std::numeric_limits<float>::max();
    } // End namespace constants

    // Utilities

#if defined(__WIN32__)
    FORCEINLINE bool  isinf(const float x) noexcept { return _finite(x) == 0; }

    FORCEINLINE bool  isnan(const float x) noexcept { return _isnan(x) != 0; }

    FORCEINLINE bool  isfinite(const float x) noexcept { return _finite(x) != 0; }
#endif

    FORCEINLINE int   to_int  (const float& a) noexcept { return int(a); }

    FORCEINLINE float to_float(const int& a) noexcept { return float(a); }

    FORCEINLINE float sqr(const float x) noexcept { return x * x; }

    FORCEINLINE float rcp(const float x) noexcept
    { 
        const __m128 a = _mm_set_ss(x); 
        const __m128 r = _mm_rcp_ss(a); 

#if defined(__AVX2__)
        return _mm_cvtss_f32(_mm_mul_ss(r,_mm_fnmadd_ss(r, a, _mm_set_ss(2.0f))));
#else
        return _mm_cvtss_f32(_mm_mul_ss(r,_mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(r, a))));
#endif
    }

    FORCEINLINE float rcp_safe(float a) noexcept { return 1.0f / a; }

    FORCEINLINE float min(float a, float b) noexcept { return a < b ? a : b; }

    FORCEINLINE float max(float a, float b) noexcept { return a > b ? a : b; }

    FORCEINLINE float fit(float s, float a1, float a2, float b1, float b2) noexcept { return b1 + ((s - a1) * (b2 - b1)) / (a2 - a1); }

    FORCEINLINE float fit01(float x, float a, float b) noexcept { return x * (b - a) + a; }

    FORCEINLINE float lerp(float a, float b, float t) noexcept { return (1.0f - t) * a + t * b; }

    FORCEINLINE float clamp(float n, float lower = constants::zero, float upper = constants::one) noexcept { return max(lower, min(n, upper)); }

    FORCEINLINE float clampz(float n, float upper = constants::one) noexcept { return max(constants::zero, min(n, upper)); }

    FORCEINLINE float deg2rad(const float deg) noexcept { return deg * constants::pi / 180.0f; }

    FORCEINLINE float rad2deg(const float rad) noexcept { return rad * 180.0f / constants::pi; }

    FORCEINLINE float abs(const float x) noexcept { return ::fabsf(x); }

    FORCEINLINE float exp(const float x) noexcept { return ::expf(x); }

    FORCEINLINE float sqrt(const float x) noexcept { return ::sqrtf(x); }

    FORCEINLINE float rsqrt(const float x) noexcept
    {
        const __m128 a = _mm_set_ss(x);
        __m128 r = _mm_rsqrt_ss(a);
        r = _mm_add_ss(_mm_mul_ss(_mm_set_ss(1.5f), r), _mm_mul_ss(_mm_mul_ss(_mm_mul_ss(a, _mm_set_ss(-0.5f)), r), _mm_mul_ss(r, r)));
        return _mm_cvtss_f32(r);
    }

    FORCEINLINE float fmod(const float x, const float y) noexcept { return ::fmodf(x, y); }

    FORCEINLINE float log(const float x) noexcept { return ::logf(x); }

    FORCEINLINE float log10(const float x) noexcept { return ::log10f(x); }

    FORCEINLINE float pow(const float x, const float y) noexcept { return ::powf(x, y); }

    FORCEINLINE float floor(const float x) noexcept { return ::floorf(x); }

    FORCEINLINE float ceil(const float x) noexcept { return ::ceilf(x); }

    FORCEINLINE float frac(const float x) noexcept { return x - ::floorf(x); }
    
    FORCEINLINE float acos(const float x) noexcept { return ::acosf(x); }

    FORCEINLINE float asin(const float x) noexcept { return ::asinf(x); }

    FORCEINLINE float atan(const float x) noexcept { return ::atanf(x); }

    FORCEINLINE float atan2(const float y, const float x) noexcept { return ::atan2f(y, x); }

    FORCEINLINE float cos(const float x) noexcept { return ::cosf(x); }

    FORCEINLINE float sin(const float x) noexcept { return ::sinf(x); }

    FORCEINLINE float tan(const float x) noexcept { return ::tanf(x); }

    FORCEINLINE float cosh(const float x) noexcept { return ::coshf(x); }

    FORCEINLINE float sinh(const float x) noexcept { return ::sinhf(x); }
    
    FORCEINLINE float tanh(const float x) noexcept { return ::tanhf(x); }

#if defined(__AVX2__)
    FORCEINLINE float madd  ( const float a, const float b, const float c) noexcept { return _mm_cvtss_f32(_mm_fmadd_ss(_mm_set_ss(a),_mm_set_ss(b),_mm_set_ss(c))); }

    FORCEINLINE float msub  ( const float a, const float b, const float c) noexcept { return _mm_cvtss_f32(_mm_fmsub_ss(_mm_set_ss(a),_mm_set_ss(b),_mm_set_ss(c))); }
    
    FORCEINLINE float nmadd ( const float a, const float b, const float c) noexcept { return _mm_cvtss_f32(_mm_fnmadd_ss(_mm_set_ss(a),_mm_set_ss(b),_mm_set_ss(c))); }
    
    FORCEINLINE float nmsub ( const float a, const float b, const float c) noexcept { return _mm_cvtss_f32(_mm_fnmsub_ss(_mm_set_ss(a),_mm_set_ss(b),_mm_set_ss(c))); }
#else
    FORCEINLINE float madd  ( const float a, const float b, const float c) noexcept { return ::fmaf(a, b, c); }
    
    FORCEINLINE float msub  ( const float a, const float b, const float c) noexcept { return a * b - c; } 
    
    FORCEINLINE float nmadd ( const float a, const float b, const float c) noexcept { return -a * b + c;}
    
    FORCEINLINE float nmsub ( const float a, const float b, const float c) noexcept { return -a * b - c; }
#endif

} // End namespace maths