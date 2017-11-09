#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>
#include <vector>

class ResourceMeshLoader;
class ResourceTextureLoader;

class ResourceManager : public Module
{
public:
	ResourceManager(bool start_enabled = true);
	virtual ~ResourceManager();

	Resource * Get(std::string unique_id);
	Resource* CreateNewResource(ResourceType type, std::string unique_id = "");
	void DeleteResource(std::string unique_id);

	void SaveResourceIntoFile(Resource* res);
	Resource* LoadResource(const char* file_path);

private:

public:

private:
	std::map<std::string, Resource*> resources;

	ResourceMeshLoader*    mesh_loader = nullptr;
	ResourceTextureLoader* texture_loader = nullptr;
	

};

#endif