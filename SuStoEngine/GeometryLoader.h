#ifndef __GEOMETRYLOADER_H__
#define __GEOMETRYLOADER_H__

#include "Module.h"
#include "GeometryMath.h"

#include "Assimp\include\DefaultLogger.hpp"

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
	Mesh(float* vertices, uint num_vertices, uint* indices, uint num_indices, float* uvs, uint num_uvs);

	void CleanUp();

	uint GetIdVertices();
	uint GetNumVertices();
	uint GetIdIndices();
	uint GetNumIndices();
	uint GetIdUV();
	uint GetNumUVs();
	AABB GetBBox();

	void LoadToMemory();
	void UnloadFromMemory();

private:
	uint   id_vertices = 0; // id in VRAM
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0; // id in VRAM
	uint   num_indices = 0;
	uint*  indices = nullptr;

	uint   id_uv = 0;
	uint   num_uvs = 0;
	float* uvs = nullptr;

	AABB   bbox;
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

private:
	vector<Mesh*> meshes;

public:

};

#endif // __GEOMETRYLOADER_H__