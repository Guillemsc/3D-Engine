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

	void SetFaces(float* vertices, uint num_vertices, uint* indices, uint num_indices);
	void SetUvs(float* uvs, uint num_uvs);
	void SetTransform(float3 pos, Quat rotation, float3 scale);

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

	float3 GetPosition();
	Quat   GetRotation();
	float3 GetScale();

	void Render();

private:
	void CalcMeshBBox();

	void LoadToMemory();
	void UnloadFromMemory();

private:
	uint   id_vertices = 0;
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0;
	uint   num_indices = 0;
	uint*  indices = nullptr;

	uint   id_uv = 0;
	uint   num_uvs = 0;
	float* uvs = nullptr;

	float3 position = float3(0, 0, 0);
	Quat   rotation = Quat(0, 0, 0, 0);
	float3 scale = float3(0, 0, 0);

	AABB   bbox;
};

#endif