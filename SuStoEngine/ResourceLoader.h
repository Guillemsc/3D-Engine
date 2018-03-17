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
	ResourceLoader(ResourceType resources_to_load, std::string assets_path, std::string library_path);
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

	virtual Resource* CreateResource(std::string new_uid) { return nullptr; };

	// LOADER ---------------------------------------------------

	ResourceType GetLoaderType();

	// Loads a file from outside the engine as a new resource
	virtual bool LoadToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources) { return false; };

	// Unloads a loaded resource from the engine
	virtual bool UnloadFromEngine(DecomposedFilePath decomposed_file_path) { return false; };

	// Removes elements of a GameObject that use this resource
	virtual void ClearFromGameObject(Resource* resource, GameObject* go) {  };

	// Exports resource into the library folder with own format
	virtual bool ExportToLibrary(Resource* resource) { return false; };

	// Imports the resource into memory from the library folder
	virtual bool ImportFromLibrary(const char* uid) { return false; };

	// Loads a resource into the scene as s new game object
	virtual bool LoadLibraryResourceIntoScene(DecomposedFilePath decomposed_file_path) { return false; };

	// Returns true if the resource can be found on the library folder
	virtual bool IsResourceOnLibrary(Resource* resource) { return false; };

	// Returns true if the resource can be found on the assets folder
	virtual bool IsResourceOnAssets(Resource* resource) { return false; };

	// Exports again resources that are missing on the assets folder, but that are on the library folder
	virtual void CreateResourcesMissingOnAssets() {};

	// Removes resources that are on the asets folder, but missing on the library folder
	virtual void RemoveResourcesMissingOnLibrary() {};

protected:
	ResourceType resources_to_load = ResourceType::RT_NULL;
	std::string assets_path;
	std::string library_path;

	std::vector<std::string> assets_extensions_to_load;
	std::vector<std::string> library_extensions_to_load;

	std::map<std::string, Resource*> resources;
};

#endif //!__RESOURCE_LOADER_H__