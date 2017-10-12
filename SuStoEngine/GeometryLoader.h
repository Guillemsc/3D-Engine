#ifndef __GEOMETRYLOADER_H__
#define __GEOMETRYLOADER_H__

#include "Module.h"

struct Mesh
{
	Mesh() {};
	Mesh(uint id_vertices, uint num_indices, uint* indices, uint id_indices, uint num_vertices, float* vertices);

	bool operator == (Mesh mesh)
	{
		bool ret = false;

		if (id_vertices == mesh.id_vertices && num_indices == mesh.num_indices && indices == mesh.indices
			&& id_indices == mesh.id_indices && num_vertices == mesh.num_vertices && vertices == mesh.vertices)
			ret = true;

		return ret;
	}

	void LoadToMemory();
	void UnloadFromMemory();
	void Draw();

	uint   id_vertices = 0; // id in VRAM
	uint   num_indices = 0;
	uint*  indices = nullptr;

	uint   id_indices = 0; // id in VRAM
	uint   num_vertices = 0;
	float* vertices = nullptr;
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
	void OnLoadFile(const char* file_path, const char* file_extension);

	bool LoadFile(const char* full_path);
	void UnloadFile(Mesh mesh);
	void UnloadAllFiles();

private:
	vector<Mesh> meshes;

public:

};

#endif // __GEOMETRYLOADER_H__