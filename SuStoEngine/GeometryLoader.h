#ifndef __GEOMETRYLOADER_H__
#define __GEOMETRYLOADER_H__

#include "Module.h"

class Mesh
{
public:
	Mesh(uint _num_vertices, float* _vertices, uint _num_indices, uint* _indices);

	bool operator == (Mesh mesh);

	void CleanUp();

	uint GetIdVertices();
	uint GetNumVertices();
	float* GetVertices();

	uint GetIdIndices();
	uint GetNumIndices();
	uint* GetIndices();

private:
	void LoadToMemory();
	void UnloadFromMemory();

private:
	uint   id_vertices = 0; // id in VRAM
	uint   num_vertices = 0;
	float* vertices = nullptr;

	uint   id_indices = 0; // id in VRAM
	uint   num_indices = 0;
	uint*  indices = nullptr;
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