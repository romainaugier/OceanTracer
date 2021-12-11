#include "camera.h"

void Camera::Update(int& xres, int& yres) noexcept
{
	aspect = (float)xres / (float)yres;

	fov = 2 * maths::rad2deg(maths::atan(36.0f / (2 * focal_length)));
	scale = maths::tan(maths::deg2rad(fov * 0.5f));
}

void Camera::SetTransform() noexcept
{
	mat44 translate_matrix = mat44();
	mat44 rotate_matrix = mat44();

	set_translation(translate_matrix, pos);

	transformation_matrix = translate_matrix * rotate_matrix;
}

void Camera::SetTransformFromCam(const mat44& rotate_matrix) noexcept
{
	mat44 translate_matrix = mat44();

	set_translation(translate_matrix, pos);

	transformation_matrix = rotate_matrix * translate_matrix;
}