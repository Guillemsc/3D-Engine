#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "Resource.h"
#include "ModuleFileSystem.h"
#include <map>
#include <vector>
#include "Math/float3.h"

class GameObject;

class ResourceLoader
{
public:
	ResourceLoader(ResourceType resources_to_load, std::string library_path, std::string loader_name);
	virtual ~ResourceLoader();

	virtual void Start() {};

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
	virtual bool LoadFileToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources) = 0;

	// Unloads all the asset engine information
	virtual bool RemoveAssetInfoFromEngine(DecomposedFilePath decomposed_file_path) = 0;

	// Removes elements of a GameObject that use this resource
	virtual void ClearFromGameObject(Resource* resource, GameObject* go) = 0;

	// Exports resource into the library folder with own format
	virtual bool ExportResourceToAssets(Resource* resource) = 0;

	// Exports resource into the library folder with own format
	virtual bool ExportResourceToLibrary(Resource* resource) = 0;

	// Imports the resource into memory from the library folder
	virtual bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path) = 0;

	// Loads a resource into the scene as s new game object
	virtual bool LoadAssetIntoScene(DecomposedFilePath decomposed_file_path) = 0;

	// Returns true if the resource can be found on the library folder
	virtual bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used) = 0;

	// Renames the library resource and all other necessary files
	virtual bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name) = 0;

	// Apparence
	float3 GetResourceColour() const;

protected:
	ResourceType resources_to_load = ResourceType::RT_NULL;
	std::string library_path;

	std::vector<std::string> assets_extensions_to_load;
	std::vector<std::string> library_extensions_to_load;

	std::map<std::string, Resource*> resources;

	std::string loader_name;
	float3 resource_colour = float3::zero;
};

#endif //!__RESOURCE_LOADER_H__