#ifndef __RESOURCE_MESH_LOADER_H__
#define __RESOURCE_MESH_LOADER_H__

#include "ResourceLoader.h"
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
		CONSOLE_LOG(message);
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

class ResourceMeshLoader : public ResourceLoader
{
public:
	ResourceMeshLoader();
	virtual ~ResourceMeshLoader();

	// NEW
	Resource* CreateResource(std::string new_uid);

	bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);

	bool UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path);
	void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportResourceToLibrary(Resource* resource);
	bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path);

	bool LoadAssetResourceIntoScene(DecomposedFilePath decomposed_file_path);

	bool IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);


private:
	void RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb, std::vector<Resource*>& resources, GameObject* parent = nullptr);
	bool ResourceIsUsed(int index, ResourceType type, Resource*& res);
	void AddResource(int index, ResourceType type, Resource* res);
	
private:
	std::vector<UsedResource> used_resources;
};

#endif