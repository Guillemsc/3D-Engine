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
	Mesh(uint id_vertices, uint num_vertices, uint id_indices, uint num_indices, uint id_uv, uint num_uvs, AABB bbox);

	void CleanUp();

	uint GetIdVertices();
	uint GetNumVertices();
	uint GetIdIndices();
	uint GetNumIndices();
	uint GetIdUV();
	uint GetNumUVs();
	AABB GetBBox();

private:
	uint   id_vertices = 0; // id in VRAM
	uint   num_vertices = 0;

	uint   id_indices = 0; // id in VRAM
	uint   num_indices = 0;

	uint   id_uv = 0;
	uint   num_uvs = 0;

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