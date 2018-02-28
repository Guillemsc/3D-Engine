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

	bool Start();
	bool CleanUp();

	// Resource management
	Resource * Get(std::string unique_id);
	Resource * Get(std::string unique_id, ResourceType type);

	ResourceLoader* GetLoader(ResourceType type);
	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	Resource* CreateNewResource(ResourceType type);
	Resource* CreateNewResource(ResourceType type, std::string unique_id);
	bool DeleteResource(std::string unique_id);

	// Resource load, unload, export, import
	void LoadResourceToEngine(const char* file_path);
	void UnloadResourceFromEngine(Resource* resource);
	void ExportResourceToLibrary(Resource* resource);
	void ImportResourceFromLibrary(const char* uid);
	void LoadResourceIntoScene(Resource* resource);
	bool IsResourceOnLibrary(Resource* resource);
	bool IsResourceOnAssets(Resource* resource);
	void CreateResourcesMissingOnAssets();
	void RemoveResourcesMissingOnLibrary();

	void SaveResourceIntoFile(Resource* res);
	bool LoadResource(const char* file_path);
	bool LoadResource(const char* file_path, vector<Resource*>& resources);
	void ImportAllResources();
	void LoadFileIntoScene(const char* file_path);
	void DeImportFile(const char* file_path);

	std::string GetNewUID();

private:
	void AddLoader(ResourceLoader* loader);

	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);
	void DeleteAllResources();

private:
	std::vector<ResourceLoader*> loaders;	
};

#endif