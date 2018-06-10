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

	bool ExportAssetToLibrary(DecomposedFilePath decomposed_file_path);
	bool ExportResourceToLibrary(Resource* resource);

	bool ImportResourceFromLibrary(const char* uid);

	bool LoadAssetResourceIntoScene(DecomposedFilePath decomposed_file_path);

	bool IsAssetOnLibrary(DecomposedFilePath d_filepath);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);

	/*
	virtual bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources) { return false; };

	// Unloads a loaded resource from the engine
	virtual bool UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path) { return false; };

	// Exports an asset to the library
	virtual bool ExportAssetToLibrary(DecomposedFilePath decomposed_file_path) { return false; };

	// Exports resource into the library folder with own format
	virtual bool ExportResourceToLibrary(Resource* resource) { return false; };

	// Imports the resource into memory from the library folder
	virtual bool ImportResourceFromLibrary(const char* uid) { return false; };

	// Loads a resource into the scene as s new game object
	virtual bool LoadAssetResourceIntoScene(DecomposedFilePath decomposed_file_path) { return false; };

	// Returns true if the resource can be found on the library folder
	virtual bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path) { return false; };

	// Renames the library resource and all other necessary files
	virtual bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name) { return false; };
	*/

private:
	void RecursiveLoadMesh(const aiScene* scene, aiNode* node, const char* full_path, AABB& total_abb, std::vector<Resource*>& resources, GameObject* parent = nullptr);
	bool ResourceIsUsed(int index, ResourceType type, Resource*& res);
	void AddResource(int index, ResourceType type, Resource* res);
	
private:
	std::vector<UsedResource> used_resources;
};

#endif