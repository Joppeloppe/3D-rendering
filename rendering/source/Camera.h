//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

class camera_t
{
	
	float vfov, aspect;	// aperture attributes
	float zNear, zFar;	// clip planes
	vec3f position;
	float roll, pitch, yaw;


public:

	//Constructor
	camera_t(
		float vfov,
		float aspect,
		float zNear,
		float zFar):
		vfov(vfov), aspect(aspect), zNear(zNear), zFar(zFar)
	{
		roll = 0;
		pitch = 0;
		yaw = 0;
	}

	//Camera movement function
	void moveTo(const vec3f& p)
	{
		position = p;
	}
	void moveUpDown(const vec3f& v)
	{
		position += v;
	}
	void RotateY(const float& angle)
	{
		yaw += angle;
	}
	void Pitch(const float& angle)
	{
		pitch += angle;
	}
	void cMoveForBackward(const float& vel_DT, int val = 1)
	{
		vec4f tr = { 0.f, 0.f, (float)val * vel_DT, 0.f };

		position += (get_ViewToWorld() * tr).xyz();
	}
	void cMoveRigtLeft(const float& vel_DT, int val = 1)
	{

		vec4f tr = { (float)val * vel_DT, 0,0,0 };

		position += (get_ViewToWorld() * tr).xyz();
	}
	//

	//Reset camera function
	void resetCamera()
	{
		position = { 0, 0, 5 };
		pitch = 0;
		yaw = 0;
		roll = 0;
	}


	//Get functions
	mat4f get_WorldToViewMatrix()
	{
		//return   mat4f::translation(-position);
		mat4f rot = mat4f::rotation(0, yaw, pitch);
		return linalg::transpose(rot) * mat4f::translation(-position);
	}
	mat4f get_ProjectionMatrix() const
	{
		return mat4f::projection(vfov, aspect, zNear, zFar);
	}
	vec3f Get_CameraPos()
	{
		return position;
	}

	

private:

	mat4f get_ViewToWorld()
	{
		//return   mat4f::translation(position) * (mat4f::rotation(yaw, 0,1,0) * mat4f::rotation(pitch, 1,0,0));
		return get_WorldToViewMatrix().inverse();
	}
};

#endif