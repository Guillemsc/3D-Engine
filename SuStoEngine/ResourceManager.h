#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>
#include <vector>
#include "ResourceLoader.h"

class ResourceMeshLoader;
class ResourceMesh;
class ResourceTextureLoader;

class ResourceManager : public Module
{
public:
	ResourceManager(bool start_enabled = true);
	virtual ~ResourceManager();

	bool Awake();
	bool Start();
	bool CleanUp();

	void OnLoadFile(const char* filepath);

	// Resource management
	Resource * Get(std::string unique_id);
	Resource * Get(std::string unique_id, ResourceType type);

	ResourceLoader* GetLoader(ResourceType type);
	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	Resource* CreateNewResource(ResourceType type);
	Resource* CreateNewResource(ResourceType type, std::string unique_id);
	bool DeleteResource(std::string unique_id);

	// Resource, assets and library management
	void LoadFileToEngine(const char* filepath);
	void UnloadAssetFromEngine(const char* filepath);
	void ExportAssetToLibrary(const char* filepath);
	void ClearResourceFromGameObjects(Resource* res);
	void ExportResourceToLibrary(Resource* resource);
	void ImportResourceFromLibrary(const char* uid);
	void LoadAssetResourceIntoScene(const char* filepath);
	bool IsAssetOnLibrary(const char* filepath);
	void CreateResourcesMissingOnLibrary();
	void RemoveResourcesMissingOnAssets();
	void RenameLibraryResource(const char* filepath, const char* new_name);

	std::string GetNewUID();

private:
	void AddLoader(ResourceLoader* loader);

	//void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);
	//void DeleteAllResources();

private:
	std::vector<ResourceLoader*> loaders;	
};

#endif