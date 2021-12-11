#pragma once

#include "ray.h"
#include <algorithm>

struct BoundingBox
{
	vec3 p0;
	vec3 p1;

	BoundingBox()
	{
		p0 = vec3(maths::constants::max_float);
		p1 = vec3(maths::constants::min_float);
	}

	BoundingBox(const vec3& a, const vec3& b) :
		p0(a),
		p1(b)
	{}
};

bool Slabs(const BoundingBox& bbox, RayHit& rayhit) noexcept;
bool Slabs(const BoundingBox& bbox, const vec3& origin, const vec3& invDir) noexcept;

BoundingBox Union(const BoundingBox& x, const BoundingBox& y) noexcept;
vec3 Offset(const BoundingBox& bbox, const vec3& p) noexcept;
float SurfaceArea(const BoundingBox& b) noexcept;
uint8_t MaximumDimension(const BoundingBox& a) noexcept;