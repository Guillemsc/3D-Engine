#ifndef __GEOMETRYLOADER_H__
#define __GEOMETRYLOADER_H__

#include "Module.h"

struct Mesh
{
	Mesh() {};
	Mesh(uint id_vertices, uint num_indices, uint* indices, uint id_indices, uint num_vertices, float* vertices);

	void LoadToMemory();
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

	bool LoadFile(const char* full_path);

private:
	vector<Mesh> meshes;
public:

};

#endif // __GEOMETRYLOADER_H__