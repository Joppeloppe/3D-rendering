//
//  Geometry.h
//
//  (c) CJG 2016, cjgribel@gmail.com
//

#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "stdafx.h"
#include <vector>
#include "vec\vec.h"
#include "vec\mat.h"
#include "ShaderBuffers.h"
#include "drawcall.h"
#include "mesh.h"

using namespace linalg;

class Geometry_t
{
protected:

	// pointers to device vertex & index arrays
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

public:

	Geometry_t() { }

	//
	// Map and update the matrix buffer
	//
	virtual void MapMatrixBuffers(
		ID3D11DeviceContext* device_context,
		ID3D11Buffer* matrix_buffer,
		mat4f ModelToWorldMatrix,
		mat4f WorldToViewMatrix,
		mat4f ProjectionMatrix);

	 void MapMatirialBuffers(
		ID3D11DeviceContext* device_context,
		ID3D11Buffer* matirial_buffer,
		vec4f specular,
		vec4f ambient,
		vec4f diffuse,
		vec3f ligthDir,
		float specularPower,
		float ligtintensity,
		vec3f cameraPos);

	virtual void render(ID3D11DeviceContext* device_context) const = 0;

	virtual ~Geometry_t()
	{ 
		// release the Krak-..device buffers
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
	}
};

class Quad_t : public Geometry_t
{
	// our local vertex and index arrays
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;

public:

	Quad_t(ID3D11Device* device);

	void render(ID3D11DeviceContext* device_context) const;

	~Quad_t() { }
};

class Cube_t : public Geometry_t
{
	// our local vertex and index arrays
	std::vector<vertex_t> vertices;
	std::vector<unsigned> indices;
	unsigned nbr_indices = 0;

public:
	vec3f local_pos;
	vec3f local_scale;
	vec3f local_rotation;

	Cube_t* cube_parent;

	Cube_t(ID3D11Device* device);

	mat4f GetModelToWorldMat() 
	{
		if (cube_parent != nullptr)
			return  cube_parent->GetModelToWorldMat()* (mat4f::scaling(local_scale)*
				   mat4f::rotation((PI / 4) *local_rotation.x, (PI / 4) *local_rotation.y, (PI / 4) *local_rotation.z) *
				   mat4f::translation(-local_pos));
		else
			return
			mat4f::scaling(local_scale)*
			mat4f::rotation((PI / 4) *local_rotation.x, (PI / 4) *local_rotation.y, (PI / 4) *local_rotation.z) *
			mat4f::translation(-local_pos);

	}

	void render(ID3D11DeviceContext* device_context) const;
	void updateRender(ID3D11DeviceContext* device_context, ID3D11Buffer* matrix_buffer, mat4f WorldToViewMatrix, mat4f ProjectionMatrix);

	~Cube_t() { }
};



class OBJModel_t : public Geometry_t
{
	// index ranges, representing drawcalls, within an index array
	struct index_range_t
	{
		size_t start;
		size_t size;
		unsigned ofs;
		int mtl_index;
	};

	std::vector<index_range_t> index_ranges;
	std::vector<material_t> materials;

	void append_materials(const std::vector<material_t>& mtl_vec)
	{
		materials.insert(materials.end(), mtl_vec.begin(), mtl_vec.end());
	}

public:

	OBJModel_t(
		const std::string& objfile,
		ID3D11Device* device);

	void render(ID3D11DeviceContext* device_context) const;

	~OBJModel_t() { }
};

#endif