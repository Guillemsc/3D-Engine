#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Module.h"
#include "Resource.h"
#include <map>
#include <vector>
#include "ResourceLoader.h"
#include "ModuleAsyncTasks.h"

class ResourceMeshLoader;
class ResourceMesh;
class ResourceTextureLoader;

class CheckCorrectLibraryAsyncTask : public AsyncTask
{
public:
	CheckCorrectLibraryAsyncTask(AsyncTaskMode mode, uint iterations_per_frame);

	void Start();
	void Update();
	void Finish();

private:
	void CheckAssetFiles();
	void CheckAssetMetaFiles();
	void ReimportFiles();
	void DeleteUnnecessary();

private:
	int asset_files_to_check_count = 0;
	std::vector<std::string> asset_files_to_check;

	int assets_to_reimport_count = 0;
	std::vector<std::string> assets_to_reimport;
	std::vector<std::string> library_files_used;

	int library_files_to_check_count = 0;
	std::vector<std::string> library_files_to_check;

	int asset_metas_to_check_count = 0;
	std::vector<std::string> asset_metas_to_check;

	bool check_asset_files = false;
	bool check_asset_meta_files = false;
	bool reimport_files = false;
	bool delete_unnecessary = false;
};

void OnCheckCorrectLibraryFinish(AsyncTask* task);


class LoadLibraryAsyncTask : public AsyncTask
{
public:
	LoadLibraryAsyncTask(AsyncTaskMode mode, uint iterations_per_frame);

	void Start();
	void Update();
	void Finish();

private:
	int library_files_to_load_count = 0;
	std::vector<std::string> library_files_to_load;
};

class ResourceManager : public Module
{
public:
	ResourceManager(bool start_enabled = true);
	virtual ~ResourceManager();

	bool Awake();
	bool Start();
	bool CleanUp();

	void OnStartEngine();
	void OnLoadFile(const char* filepath);

	ResourceLoader* GetLoader(ResourceType type);
	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	// Resource management
	Resource* Get(std::string unique_id);
	Resource* Get(std::string unique_id, ResourceType type);

	Resource* CreateNewResource(ResourceType type);
	Resource* CreateNewResource(ResourceType type, std::string unique_id);
	bool DeleteResource(std::string unique_id);

	// Resource, assets and library management
	void LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	void UnloadAssetFromEngine(const char* filepath);
	void RemoveAssetInfoFromEngine(const char* filepath);
	void ClearResourceFromGameObjects(Resource* res);
	void ExportResourceToLibrary(Resource* resource);
	void ImportResourceFromLibrary(const char* uid);
	void LoadAssetResourceIntoScene(const char* filepath);
	bool IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used = std::vector<std::string>());
	void ReimportAssetToEngine(const char* filepath);
	void RenameAsset(const char* filepath, const char* new_name);

	// Meta control
	bool IsAssetMeta(const char* filepath, const char* metapath);
	bool FileIsMeta(const char* filepath);

	std::string GetNewUID();

	bool DrawResourceSelector(const char* name, ResourceType type, Resource*& res);

private:
	void AddLoader(ResourceLoader* loader);

	//void DeleteAllResources();

private:
	std::vector<ResourceLoader*> loaders;	

	bool draw_resource_selector_win = false;
};

#endif