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

	bool RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path);
	void ClearFromGameObject(Resource* resource, GameObject* go);

	bool ExportResourceToLibrary(Resource* resource);
	bool ImportResourceFromLibrary(const char* uid);

	bool LoadAssetResourceIntoScene(Resource* resource);

	bool IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);

	ResourcePrefab* CreatePrefab(GameObject* go);
	void UpdatePrefab(const char* prefab_filepath, GameObject* go);

private:

private:

};

#endif