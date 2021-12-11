#pragma once

#include "maths.h"

struct alignas(16) vec2
{
	float x, y;

	vec2() : x(0.0f), y(0.0f) {}
	vec2(float t) : x(t), y(t) {}
	vec2(float X, float Y) : x(X), y(Y) {}

	const vec2 operator-() const { return vec2(-x, -y); }

	const float& operator [] (int i) const { return (&x)[i]; }
	float& operator [] (int i) { return (&x)[i]; }
};

// maths operators
FORCEINLINE vec2 operator+(const vec2& vec, const vec2& other) noexcept { return vec2(vec.x + other.x, vec.y + other.y); }
FORCEINLINE vec2 operator+(const vec2& vec, const float t) noexcept { return vec2(vec.x + t, vec.y + t); }
FORCEINLINE vec2 operator-(const vec2& vec, const vec2& other) noexcept { return vec2(vec.x - other.x, vec.y - other.y); }
FORCEINLINE vec2 operator-(const vec2& vec, const float t) noexcept { return vec2(vec.x - t, vec.y - t); }
FORCEINLINE vec2 operator*(const vec2& vec, const vec2& other) noexcept { return vec2(vec.x * other.x, vec.y * other.y); }
FORCEINLINE vec2 operator*(const vec2& vec, const float t) noexcept { return vec2(vec.x * t, vec.y * t); }
FORCEINLINE vec2 operator*(const float t, const vec2& vec) noexcept { return vec2(vec.x * t, vec.y * t); }
FORCEINLINE vec2 operator/(const vec2& vec, const vec2& other) noexcept { return vec2(vec.x / other.x, vec.y / other.y); }
FORCEINLINE vec2 operator/(const vec2& vec, const float t) noexcept { return vec2(vec.x / t, vec.y / t); }
FORCEINLINE vec2 operator/(const float t, const vec2& vec) noexcept { return vec2(t / vec.x, t / vec.y); }

FORCEINLINE bool operator==(const vec2& v0, const vec2& v1) noexcept { if (v0.x == v1.x && v0.y == v1.y) return true; else return false; }
FORCEINLINE bool operator>(const vec2& v0, const vec2& v1) noexcept { if (v0.x > v1.x && v0.y > v1.y) return true; else return false; }
FORCEINLINE bool operator>=(const vec2& v0, const vec2& v1) noexcept { if (v0.x >= v1.x && v0.y >= v1.y) return true; else return false; }
FORCEINLINE bool operator<(const vec2& v0, const vec2& v1) noexcept { if (v0.x < v1.x && v0.y < v1.y) return true; else return false; }
FORCEINLINE bool operator<=(const vec2& v0, const vec2& v1) noexcept { if (v0.x <= v1.x && v0.y <= v1.y) return true; else return false; }

FORCEINLINE vec2 operator+=(vec2& v0, vec2 v1) noexcept { v0 = v0 + v1; return v0; }
FORCEINLINE vec2 operator-=(vec2& v0, vec2 v1) noexcept { v0 = v0 - v1; return v0; }
FORCEINLINE vec2 operator*=(vec2& v0, vec2 v1) noexcept { v0 = v0 * v1; return v0; }
FORCEINLINE vec2 operator*=(vec2& v0, float t) noexcept { v0 = v0 * t; return v0; }
FORCEINLINE vec2 operator/=(vec2& v0, vec2 v1) noexcept { v0 = v0 / v1; return v0; }
FORCEINLINE vec2 operator/=(vec2& v0, float t) noexcept { float inv = maths::rcp(t); return vec2(v0.x *= inv, v0.y *= inv); }

// utility functions
FORCEINLINE float dot(const vec2& v1, const vec2& v2) noexcept { return v1.x * v2.x + v1.y * v2.y; }
FORCEINLINE float length(const vec2& a) noexcept { return maths::sqrt(dot(a, a)); }
FORCEINLINE float length2(const vec2& a) noexcept { return dot(a, a); }
FORCEINLINE vec2  normalize_safe(const vec2& v) noexcept { float t = 1.0f / length(v); return vec2(v.x * t, v.y * t); }
FORCEINLINE vec2  normalize(const vec2& v) noexcept { float t = maths::rsqrt(dot(v, v)); return vec2(v.x * t, v.y * t); }
FORCEINLINE float dist(const vec2& a, const vec2& b) noexcept { return maths::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }
FORCEINLINE vec2  abs(const vec2& a) noexcept { return vec2(maths::abs(a.x), maths::abs(a.y)); }
FORCEINLINE vec2  sum(const vec2& v1, const vec2& v2, const vec2& v3) noexcept { return vec2((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3); }
FORCEINLINE vec2  powvec2(const vec2& v, const float p) noexcept { return vec2(maths::pow(v.x, p), maths::pow(v.y, p)); }
FORCEINLINE vec2  min(const vec2& a, const vec2& b) noexcept { return vec2(maths::min(a.x, b.x), maths::min(a.y, b.y)); }
FORCEINLINE vec2  max(const vec2& a, const vec2& b) noexcept { return vec2(maths::max(a.x, b.x), maths::max(a.y, b.y)); }
FORCEINLINE vec2  lerp(const vec2& a, const vec2& b, const float t) noexcept { return vec2(maths::lerp(a.x, b.x, t), maths::lerp(a.y, b.y, t)); }
FORCEINLINE vec2  lerp(const vec2& a, const vec2& b, const vec2& t) noexcept { return vec2(maths::lerp(a.x, b.x, t.x), maths::lerp(a.y, b.y, t.y)); }