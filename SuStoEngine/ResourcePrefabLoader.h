#ifndef __RESOURCE_PREFAB_LOADER_H__
#define __RESOURCE_PREFAB_LOADER_H__

#include "ResourceLoader.h"
#include "Resource.h"
#include <vector>

class ResourcePrefab;

class ResourcePrefabLoader : public ResourceLoader
{
public:
	ResourcePrefabLoader();
	virtual ~ResourcePrefabLoader();

	Resource* CreateResource(std::string new_uid);

	bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);
	bool UnloadAssetFromEngine(DecomposedFilePath decomposed_file_path);

	//void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportToLibrary(Resource* resource);
	//bool ImportFromLibrary(const char* uid);

	bool LoadIntoScene(Resource* resource);

	ResourcePrefab* CreatePrefab(GameObject* go);

private:

private:

};

#endif