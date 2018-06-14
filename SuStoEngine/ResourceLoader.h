#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "Resource.h"
#include "ModuleFileSystem.h"
#include <map>
#include <vector>

class GameObject;

class ResourceLoader
{
public:
	ResourceLoader(ResourceType resources_to_load, std::string library_path);
	virtual ~ResourceLoader();

	// Extensions

	void AddAssetExtensionToLoad(const char* extension);
	void AddLibraryExtensionToLoad(const char* extension);

	bool CanLoadExtensionAsset(const char* extension);
	bool CanLoadExtensionLibrary(const char* extension);

	// Resources managing

	Resource* GetResource(std::string unique_id);

	void AddResource(Resource* resource);
	bool DeleteResource(std::string unique_id);

	std::map<std::string, Resource*> GetResources();

	virtual Resource* CreateResource(std::string new_uid) { return nullptr; };

	// LOADER ---------------------------------------------------

	ResourceType GetLoaderType();

	// Loads a file from outside the engine as a new resource
	virtual bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources) { return false; };

	// Unloads all the asset engine information
	virtual bool RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path) { return false; };

	// Removes elements of a GameObject that use this resource
	virtual void ClearFromGameObject(Resource* resource, GameObject* go) {  };

	// Exports resource into the library folder with own format
	virtual bool ExportResourceToLibrary(Resource* resource) { return false; };

	// Imports the resource into memory from the library folder
	virtual bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path) { return false; };

	// Loads a resource into the scene as s new game object
	virtual bool LoadAssetIntoScene(DecomposedFilePath decomposed_file_path) { return false; };

	// Returns true if the resource can be found on the library folder
	virtual bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used) { return false; };

	// Renames the library resource and all other necessary files
	virtual bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name) { return false; };

protected:
	ResourceType resources_to_load = ResourceType::RT_NULL;
	std::string library_path;

	std::vector<std::string> assets_extensions_to_load;
	std::vector<std::string> library_extensions_to_load;

	std::map<std::string, Resource*> resources;
};

#endif //!__RESOURCE_LOADER_H__