#pragma once

#include "mat44.h"

struct Camera
{
	mat44 transformation_matrix;
	
	vec3 pos;

	float focal_length;
	float fov;
	float aspect;
	float scale;
	
	Camera() {}

	Camera(vec3 _pos, vec3 _lookat, float focal, int xres, int yres) :
		pos(_pos),
		focal_length(focal),
		aspect((float)xres / (float)yres)
	{
		transformation_matrix = mat44();

		fov = 2 * maths::rad2deg(maths::atan(36.0f / (2 * focal_length)));
		scale = maths::tan(maths::deg2rad(fov * 0.5f));
	}

	void Update(int& xres, int& yres) noexcept;

	void SetTransform() noexcept;
	
	void SetTransformFromCam(const mat44& rotate_matrix) noexcept;

};