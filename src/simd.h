#pragma once

// Simple simd library using sse, avx and avx2 intrinsics

#include "immintrin.h"
#include "intrin.h"

#include "decl.h"

using vfloat4 = __m128;
using vfloat8 = __m256;
using vint4 = __m128i;
using vint8 = __m256i;
using vmask8 = __mmask8;
using vmask16 = __mmask16;
using vmask32 = __mmask32;
using vmask64 = __mmask64;

FORCEINLINE vfloat4 load(const float* ptr) { return _mm_load_ps(ptr); }
FORCEINLINE vfloat4 loadu(const float* ptr) { return _mm_loadu_ps(ptr); }
FORCEINLINE void store(float* ptr, const vfloat4& v) { return _mm_store_ps(ptr, v); }
FORCEINLINE void storeu(float* ptr, const vfloat4& v) { return _mm_storeu_ps(ptr, v); }