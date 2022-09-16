#pragma once
#include <maths/vector4.h>

//Struct for storage of camera data
struct CameraData
{
	gef::Vector4 camera_eye_;
	gef::Vector4 camera_lookat_;
	gef::Vector4 camera_up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

};