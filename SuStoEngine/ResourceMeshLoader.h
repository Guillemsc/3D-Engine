#ifndef __RESOURCE_MESH_LOADER_H__
#define __RESOURCE_MESH_LOADER_H__

#include "Assimp\include\DefaultLogger.hpp"
#include "Assimp\include\scene.h"
#include "GeometryMath.h"
#include <string>
#include "Globals.h"
#include "Resource.h"

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

class UsedResource
{
public:
	UsedResource(Resource* _res, int _index, ResourceType _type)
	{
		res = _res;
		index = _index;
		type = _type;
	}

	Resource* GetResource() { return res; }
	int GetIndex() { return index; }
	ResourceType GetType() { return type; }

private:
	Resource* res = nullptr;
	int index = 0;
	ResourceType type = ResourceType::RT_NULL;
};

class ResourceMeshLoader
{
public:
	ResourceMeshLoader();
	virtual ~ResourceMeshLoader();

	bool Load(const char* filepath, bool as_new_gameobject = true);
	void Import(const char* filepath);
	bool Export(const char* path, ResourceMesh* mesh);
	void ImportAllMeshes();

private:
	void RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb, GameObject* parent = nullptr);
	bool ResourceIsUsed(int index, ResourceType type, Resource*& res);
	void AddResource(int index, ResourceType type, Resource* res);

private:
	std::vector<UsedResource> used_resources;
};

#endif