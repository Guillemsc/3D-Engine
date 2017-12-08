#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>
#include <vector>

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

	Resource * Get(std::string unique_id);
	Resource* CreateNewResource(ResourceType type, std::string unique_id = "");
	void DeleteResource(std::string unique_id);

	void SaveResourceIntoFile(Resource* res);
	bool LoadResource(const char* file_path);
	bool LoadResource(const char* file_path, vector<Resource*>& resources);
	void ImportAllResources();
	void LoadFileIntoScene(const char* file_path);
	void DeImportFile(const char* file_path);

	ResourceMeshLoader* GetMeshLoader();
	ResourceTextureLoader* GetTextureLoader();

	std::string GetNewUID();

private:
	void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension);
	void DeleteAllResources();

private:
	std::map<std::string, Resource*> resources;

	ResourceMeshLoader*    mesh_loader = nullptr;
	ResourceTextureLoader* texture_loader = nullptr;	
};

#endif