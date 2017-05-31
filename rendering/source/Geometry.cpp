#include "Geometry.h"


void Geometry_t::MapMatrixBuffers(
	ID3D11DeviceContext* device_context,
	ID3D11Buffer* matrix_buffer,
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// map the resource buffer, obtain a pointer to it and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	device_context->Map(matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MatrixBuffer_t* matrix_buffer_ = (MatrixBuffer_t*)resource.pData;
	matrix_buffer_->ModelToWorldMatrix = linalg::transpose(ModelToWorldMatrix);
	matrix_buffer_->WorldToViewMatrix = linalg::transpose(WorldToViewMatrix);
	matrix_buffer_->ProjectionMatrix = linalg::transpose(ProjectionMatrix);
	device_context->Unmap(matrix_buffer, 0);
}

void Geometry_t::MapMatirialBuffers(
	ID3D11DeviceContext* device_context,
	ID3D11Buffer* matirial_buffer,
	vec4f specular,
	vec4f ambient,
	vec4f diffuse,
	vec3f ligthDir,
	float specPower,
	float ligtintensity,
	vec3f cameraPos)
{
	// map the resource buffer, obtain a pointer to it and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	device_context->Map(matirial_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	MaterialBuffer_t* buffer = (MaterialBuffer_t*)resource.pData;
	buffer->specular = specular;
	buffer->ambient = ambient;
	buffer->diffuse = diffuse;
	buffer->specularPower = specPower;
	buffer->lightDirection = ligthDir;
	buffer->ligtintensity = ligtintensity;
	buffer->cameraPos = cameraPos;
	device_context->Unmap(matirial_buffer, 0);
}


Quad_t::Quad_t(ID3D11Device* device)
{
	// populate the vertex array with 4 vertices
	vertex_t v0, v1, v2, v3;
	v0.Pos = { -0.5, -0.5f, 0.0f };
	v0.Normal = { 0, 0, 1 };
	v0.TexCoord = { 0, 0 };
	v1.Pos = { 0.5, -0.5f, 0.0f };
	v1.Normal = { 0, 0, 1 };
	v1.TexCoord = { 0, 1 };
	v2.Pos = { 0.5, 0.5f, 0.0f };
	v2.Normal = { 0, 0, 1 };
	v2.TexCoord = { 1, 1 };
	v3.Pos = { -0.5, 0.5f, 0.0f };
	v3.Normal = { 0, 0, 1 };
	v3.TexCoord = { 1, 0 };
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	// populate the index array
	// triangle #1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// triangle #2
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = vertices.size()*sizeof(vertex_t);
	// data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// create vertex buffer on device using descriptor & data
	HRESULT vhr = device->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	//  index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size()*sizeof(unsigned);
	// data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// create index buffer on device using descriptor & data
	HRESULT ihr = device->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// local data is now loaded to device so it can be released
	vertices.clear();
	nbr_indices = indices.size();
	indices.clear();
}

void Quad_t::render(ID3D11DeviceContext* device_context) const
{
	//set topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind our vertex buffer
	UINT32 stride = sizeof(vertex_t); //  sizeof(float) * 8;
	UINT32 offset = 0;
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// bind our index buffer
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// make the drawcall
	device_context->DrawIndexed(nbr_indices, 0, 0);
}

Cube_t::Cube_t(ID3D11Device* device)
{
	local_scale = vec3f(1.f, 1.f, 1.f);
	local_pos = vec3f(0.f, 0.f, 0.f);
	local_rotation = vec3f(0.f, 0.f, 0.f);

	// populate the vertex array with 4 vertices
	vertex_t v0, v1, v2, v3, v4, v5, v6, v7;
	v0.Pos = { -0.5, -0.5f, 0.5f };
	v0.Normal = { 0, 0, 1 };
	v0.TexCoord = { 0, 0 };

	v1.Pos = { 0.5, -0.5f, 0.5f };
	v1.Normal = { 0, 0, 1 };
	v1.TexCoord = { 0, 1 };

	v2.Pos = { -0.5, 0.5f, 0.5f };
	v2.Normal = { 0, 0, 1 };
	v2.TexCoord = { 1, 0 };

	v3.Pos = { 0.5, 0.5f, 0.5f };
	v3.Normal = { 0, 0, 1 };
	v3.TexCoord = { 1, 1 };
	//						//
	v4.Pos = { -0.5, -0.5f, -0.5f };
	v4.Normal = { 0, 0, 1 };
	v4.TexCoord = { 0, 0 };

	v5.Pos = { 0.5, -0.5f, -0.5f };
	v5.Normal = { 0, 0, 1 };
	v5.TexCoord = { 0, 1 };

	v6.Pos = { -0.5, 0.5f, -0.5f };
	v6.Normal = { 0, 0, 1 };
	v6.TexCoord = { 1, 0 };

	v7.Pos = { 0.5, 0.5f, -0.5f };
	v7.Normal = { 0, 0, 1 };
	v7.TexCoord = { 1, 1 };

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);

	// populate the index array
	//Front
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
	//Bottom
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(5);
	//Right //klar
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(3);

	//Left // klar
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(4);
	indices.push_back(2);
	indices.push_back(6);

	//Top //Klar
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(6);
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(6);

	//Back // klar
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	

	// vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = vertices.size()*sizeof(vertex_t);
	// data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// create vertex buffer on device using descriptor & data
	HRESULT vhr = device->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	//  index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size()*sizeof(unsigned);
	// data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// create index buffer on device using descriptor & data
	HRESULT ihr = device->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// local data is now loaded to device so it can be released
	vertices.clear();
	nbr_indices = indices.size();
	indices.clear();
}

void Cube_t::render(ID3D11DeviceContext* device_context) const
{
	//set topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind our vertex buffer
	UINT32 stride = sizeof(vertex_t); //  sizeof(float) * 8;
	UINT32 offset = 0;
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// bind our index buffer
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// make the drawcall
	device_context->DrawIndexed(nbr_indices, 0, 0);
}

void Cube_t::updateRender(ID3D11DeviceContext* device_context, ID3D11Buffer* matrix_buffer, mat4f WorldToViewMatrix, mat4f ProjectionMatrix)
{

}


OBJModel_t::OBJModel_t(const std::string& objfile,ID3D11Device* device)
{
	//
	// load the OBJ
	//
	mesh_t* mesh = new mesh_t();
	mesh->load_obj(objfile);

	// load and organize indices in ranges per drawcall (material)
	//
	std::vector<unsigned> indices;
	size_t i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// append the drawcall indices
		for (auto& tri : dc.tris)
			indices.insert(indices.end(), tri.vi, tri.vi + 3);

		// create a range
		size_t i_size = dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		index_ranges.push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = indices.size();
	}

	// vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0.0f };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = mesh->vertices.size()*sizeof(vertex_t);
	// data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &(mesh->vertices)[0];
	// create vertex buffer on device using descriptor & data
	HRESULT vhr = device->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);

	// index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0.0f };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = indices.size()*sizeof(unsigned);
	// data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// create index buffer on device using descriptor & data
	HRESULT ihr = device->CreateBuffer(&ibufferDesc, &idata, &index_buffer);

	// copy materials from mesh
	append_materials(mesh->materials);

	// load textures associated with materials to device
	for (auto& mtl : materials)
	{
		HRESULT hr;
		std::wstring wstr; // for conversion from string to wstring

		// Kd_map
		if (mtl.map_Kd.size()) {
			wstr = std::wstring(mtl.map_Kd.begin(), mtl.map_Kd.end());
			hr = DirectX::CreateWICTextureFromFile(device, wstr.c_str(), &mtl.map_Kd_Tex, &mtl.map_Kd_TexSRV);
			printf("loading texture %s - %s\n", mtl.map_Kd.c_str(), SUCCEEDED(hr) ? "OK" : "FAILED");
		}

		// other maps here...
	}

	SAFE_DELETE(mesh);
}


void OBJModel_t::render(ID3D11DeviceContext* device_context) const
{
	//set topology
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind vertex buffer
	UINT32 stride = sizeof(vertex_t); //  sizeof(float) * 8;
	UINT32 offset = 0;
	//ID3D11Buffer* buffersToSet[] = { vertex_buffer };
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// bind index buffer
	device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// iterate drawcalls
	for (auto& irange : index_ranges)
	{
		// fetch material and bind texture
		const material_t& mtl = materials[irange.mtl_index];
		device_context->PSSetShaderResources(0, 1, &mtl.map_Kd_TexSRV);

		// make the drawcall
		device_context->DrawIndexed(irange.size, irange.start, 0);
	}
}