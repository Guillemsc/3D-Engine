#ifndef __RESOURCE_MESH_LOADER_H__
#define __RESOURCE_MESH_LOADER_H__

#include "Assimp\include\DefaultLogger.hpp"
#include "Assimp\include\scene.h"
#include "GeometryMath.h"
#include <string>
#include "Globals.h"

class ResourceMesh;
class GameObject;

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
		LOG_OUTPUT(message);
	}
};

class ResourceMeshLoader
{
public:
	ResourceMeshLoader();
	virtual ~ResourceMeshLoader();

	bool Load(const char* filepath, bool as_new_gameobject = true);
	void Import(const char* filepath);
	bool Export(const char* filepath, ResourceMesh* mesh);

private:
	void RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb, GameObject* parent = nullptr);

private:

};

#endif