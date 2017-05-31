
#pragma once
#ifndef MATRIXBUFFERS_H
#define MATRIXBUFFERS_H

#include "vec\vec.h"
#include "vec\mat.h"

using namespace linalg;

struct MatrixBuffer_t
{
	mat4f ModelToWorldMatrix;
	mat4f WorldToViewMatrix;
	mat4f ProjectionMatrix;
};

struct MaterialBuffer_t
{
	vec4f specular;
	vec4f ambient;
	vec4f diffuse;

	//Nytt
	vec3f lightDirection;
	float specularPower;
	float ligtintensity;
	vec3f cameraPos;
};

struct CameraBuffer_t
{
	vec3f cameraPos;
	float padding;	
};


#endif