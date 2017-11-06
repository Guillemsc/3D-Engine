#ifndef __GEOMETRYLOADER_H__
#define __GEOMETRYLOADER_H__

#include "Module.h"
#include "GeometryMath.h"

#include "Assimp\include\DefaultLogger.hpp"
#include "Assimp\include\scene.h"

class GameObject;
class MeshImporter;

class AssimpLogger : public Assimp::LogStream
{
public:
	AssimpLogger()
	{

	}
	~AssimpLogger()
	{}
	void write(const char* message)
	{
		log(__FILE__, __LINE__, "%s\n", message);
	}
};

class Mesh
{
public:
	Mesh(float* vertices, uint num_vertices, uint* indices, uint num_indices, float* uvs, uint num_uvs, const char* filename);
	Mesh();

	void CleanUp();

	void SetUniqueId(double id);
	void SetFaces(float* vertices, uint num_vertices, uint* indices, uint num_indices);
	void SetUvs(float* uvs, uint num_uvs);
	void SetTransform(float3 pos, Quat rotation, float3 scale);

	uint GetIdVertices();
	uint GetNumVertices();
	uint GetIdIndices();
	uint GetNumIndices();
	uint GetIdUV();
	uint GetNumUVs();
	AABB GetBBox();
	float GetDiagonal();

	string GetFilename();
	float* GetVertices();
	uint* GetIndices();
	float* GetUVs();

	float3 GetPosition();
	Quat   GetRotation();
	float3 GetScale();

	void LoadToMemory();
	void UnloadFromMemory();

	const double GetUniqueId() const;

private:
	void CalcMeshBBox();

private:
	string file_name;

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

	double unique_id = 0;
};

class GeometryLoader : public Module
{
public:
	GeometryLoader(bool start_enabled = true);
	~GeometryLoader();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);

	bool LoadFile(const char* full_path, bool as_new_gameobject = false);
	void UnloadFile(Mesh* mesh);
	void UnloadAllFiles();

	vector<Mesh*>* GetMeshesVector();
	MeshImporter* GetMeshImporter();

private:
	void RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb, GameObject* parent = nullptr);

private:
	vector<Mesh*> meshes;

	MeshImporter* mesh_importer = nullptr;

public:

};

#include "Importer.h"

class MeshImporter : public Importer
{
public:
	bool Load(const char * exported_file);
	bool Save(const char* path, Mesh* meshes);

	void ImportAllMeshes();
};

#endif // __GEOMETRYLOADER_H__