#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>
#include <vector>

class ResourceManager : public Module
{
public:
	ResourceManager(bool start_enabled = true);
	virtual ~ResourceManager();

	Resource * Get(double id);
	Resource* CreateNewResource(ResourceType type, double id = -1);
	void DeleteResource(double id);

	void SaveResourceIntoFile(Resource* res);
	Resource* LoadResource(const char* file_path);

private:

public:

private:
	std::map<double, Resource*> resources;

};

#endif