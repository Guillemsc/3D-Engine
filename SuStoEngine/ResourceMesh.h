#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"
#include "Globals.h"
#include "GeometryMath.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh(std::string unique_id);
	virtual ~ResourceMesh();

	void CleanUp();

	void SendToShaders();
	void Render();

	void SetFaces(float* vertices, uint num_vertices, uint* indices, uint num_indices);
	void SetUvs(float* uvs, uint num_uvs);

	uint GetIdVertices();
	uint GetNumVertices();
	uint GetIdIndices();
	uint GetNumIndices();
	uint GetIdUV();
	uint GetNumUVs();
	float* GetVertices();
	uint* GetIndices();
	float* GetUVs();
	AABB GetBBox();
	float GetDiagonal();

private:
	void CalcMeshBBox();

	void LoadToMemory();
	void UnloadFromMemory();

private:
	uint   vao_id = 0;

	uint   id_vertices = 0;
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0;
	uint   num_indices = 0;
	uint*  indices = nullptr;

	uint   id_uv = 0;
	uint   num_uvs = 0;
	float* uvs = nullptr;

	AABB   bbox;

	bool   on_shaders = false;
};

#endif