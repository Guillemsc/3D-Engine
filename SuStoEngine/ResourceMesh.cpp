#include "ResourceMesh.h"
#include "App.h"
#include "ModuleRenderer3D.h"

ResourceMesh::ResourceMesh(std::string unique_id) : Resource(unique_id, ResourceType::RT_MESH)
{
	vao_id = App->renderer3D->GenVertexArrayBuffer();
}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::CleanUp()
{
	// Unload from vram
	UnloadFromMemory();

	// Vertices
	id_vertices = 0;
	num_vertices = 0;
	RELEASE_ARRAY(vertices);

	// Indices
	id_indices = 0;
	num_indices = 0;
	RELEASE_ARRAY(indices);
}

void ResourceMesh::SendToShaders()
{
	LoadToMemory();

	vao_id = App->renderer3D->GenVertexArrayBuffer();

	App->renderer3D->BindVertexArrayBuffer(vao_id);

	App->renderer3D->BindArrayBuffer(id_vertices);

	//vertices
	App->renderer3D->SetVertexAttributePointer(0, 3, 13, 0);
	App->renderer3D->EnableVertexAttributeArray(0);

	//texture coords
	App->renderer3D->SetVertexAttributePointer(1, 3, 13, 3);
	App->renderer3D->EnableVertexAttributeArray(1);

	//normals
	App->renderer3D->SetVertexAttributePointer(2, 3, 13, 6);
	App->renderer3D->EnableVertexAttributeArray(2);

	//colors
	App->renderer3D->SetVertexAttributePointer(3, 4, 13, 9);
	App->renderer3D->EnableVertexAttributeArray(3);

	App->renderer3D->BindElementArrayBuffer(id_indices);

	App->renderer3D->UnbindVertexArrayBuffer();
}

void ResourceMesh::Render()
{
	App->renderer3D->BindVertexArrayBuffer(vao_id);

	App->renderer3D->RenderElement(num_indices);
}

void ResourceMesh::SetFaces(float * _vertices, uint _num_vertices, uint * _indices, uint _num_indices)
{
	if (_num_vertices > 0)
	{
		// Vertices
		vertices = new float[_num_vertices * 13];
		memcpy(vertices, _vertices, sizeof(float) * _num_vertices * 13);
		num_vertices = _num_vertices;

		if (_num_indices > 0)
		{
			// Indices
			indices = new uint[_num_indices];
			memcpy(indices, _indices, sizeof(uint) * _num_indices);
			num_indices = _num_indices;
		}

		CalcMeshBBox();

		SendToShaders();
	}
}

uint ResourceMesh::GetIdVertices()
{
	return id_vertices;
}

uint ResourceMesh::GetNumVertices()
{
	return num_vertices;
}

uint ResourceMesh::GetIdIndices()
{
	return id_indices;
}

uint ResourceMesh::GetNumIndices()
{
	return num_indices;
}

float * ResourceMesh::GetVertices()
{
	return vertices;
}

uint * ResourceMesh::GetIndices()
{
	return indices;
}

AABB ResourceMesh::GetBBox()
{
	return bbox;
}

float ResourceMesh::GetDiagonal()
{
	return bbox.Diagonal().Length();
}

void ResourceMesh::LoadToMemory()
{
	if (id_vertices == 0 && vertices != nullptr)
	{
		id_vertices = App->renderer3D->GenBuffer();
		App->renderer3D->BindArrayBuffer(id_vertices);
		App->renderer3D->LoadArrayToVRAM(sizeof(float) * num_vertices * 13, vertices, GL_STATIC_DRAW);
		App->renderer3D->UnbindArraybuffer();
	}

	if (id_indices == 0 && indices != nullptr)
	{
		id_indices = App->renderer3D->GenBuffer();
		App->renderer3D->BindArrayBuffer(id_indices);
		App->renderer3D->LoadArrayToVRAM(sizeof(uint) * num_indices, indices, GL_STATIC_DRAW);
		App->renderer3D->UnbindArraybuffer();

		id_indices = App->renderer3D->LoadBuffer(indices, num_indices);
	}
}

void ResourceMesh::UnloadFromMemory()
{
	if (id_vertices > 0)
	{
		App->renderer3D->UnloadBuffer(id_vertices);
		id_vertices = 0;
	}

	if (id_indices > 0)
	{
		App->renderer3D->UnloadBuffer(id_indices);
		id_vertices = 0;
	}
}

void ResourceMesh::CalcMeshBBox()
{
	bbox.SetNegativeInfinity();

	if (vertices != nullptr && num_vertices > 0)
	{
		bbox.Enclose((vec*)vertices, num_vertices);
	}
}