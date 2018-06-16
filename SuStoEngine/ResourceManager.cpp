#include "ResourceManager.h"
#include "Functions.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ResourceMeshLoader.h"
#include "ResourceTextureLoader.h"
#include "Globals.h"
#include "JSONLoader.h"
#include "ModuleGameObject.h"
#include "ResourcePrefabLoader.h"
#include "ResourcePrefab.h"
#include "ModuleEventSystem.h"
#include "ResourceShaderLoader.h"
#include "imgui.h"

ResourceManager::ResourceManager(bool start_enabled) : Module(start_enabled)
{

}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Awake()
{
	bool ret = true;

	ResourceMeshLoader* mesh_loader = new ResourceMeshLoader();
	mesh_loader->AddAssetExtensionToLoad("fbx");
	mesh_loader->AddLibraryExtensionToLoad("sustomesh");

	ResourceTextureLoader* texture_loader = new ResourceTextureLoader();
	texture_loader->AddAssetExtensionToLoad("png");
	texture_loader->AddAssetExtensionToLoad("jpg");
	texture_loader->AddAssetExtensionToLoad("tga");
	texture_loader->AddAssetExtensionToLoad("dds");
	texture_loader->AddLibraryExtensionToLoad("dds");

	ResourcePrefabLoader* prefab_loader = new ResourcePrefabLoader();
	prefab_loader->AddAssetExtensionToLoad("prefab");
	prefab_loader->AddLibraryExtensionToLoad("prefab");
	prefab_loader->AddLibraryExtensionToLoad("meshprefab");

	ResourceShaderLoader* shader_loader = new ResourceShaderLoader();
	shader_loader->AddLibraryExtensionToLoad("vert");
	shader_loader->AddLibraryExtensionToLoad("frag");

	AddLoader(mesh_loader);
	AddLoader(texture_loader);
	AddLoader(prefab_loader);
	AddLoader(shader_loader);

	OnStartEngine();

	return ret;
}

bool ResourceManager::Start()
{
	bool ret = true;

	// Start loaders
	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
		(*it)->Start();

	return ret;
}

bool ResourceManager::CleanUp()
{
	bool ret = true;

	//DeleteAllResources();

	return ret;
}

void ResourceManager::OnStartEngine()
{	
	// Start async assets check 
	CheckCorrectLibraryAsyncTask* new_task = new CheckCorrectLibraryAsyncTask(AsyncTaskMode::AST_FOCUS, 2);
	new_task->OnFinish(OnCheckCorrectLibraryFinish);
	App->async_tasks->StartTask(new_task);
}

void OnCheckCorrectLibraryFinish(AsyncTask * task)
{
	// Start async resource load 
	LoadLibraryAsyncTask* new_task = new LoadLibraryAsyncTask(AsyncTaskMode::AST_FOCUS, 2);
	App->async_tasks->StartTask(new_task);
}

void ResourceManager::OnLoadFile(const char * filepath)
{
	LoadFileToEngine(filepath);
}

ResourceLoader * ResourceManager::GetLoader(ResourceType type)
{
	ResourceLoader* ret = nullptr;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* loader = (*it);

		if (loader->GetLoaderType() == type)
		{
			ret = loader;
			break;
		}
	}

	return ret;
}

std::vector<ResourceLoader*> ResourceManager::GetLoaders() const
{
	return loaders;
}

ResourceType ResourceManager::AssetExtensionToType(const char * extension)
{
	ResourceType ret = ResourceType::RT_NULL;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it)->CanLoadExtensionAsset(extension))
		{
			ret = (*it)->GetLoaderType();
			break;
		}
	}

	return ret;
}

ResourceType ResourceManager::LibraryExtensionToType(const char * extension)
{
	ResourceType ret = ResourceType::RT_NULL;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it)->CanLoadExtensionLibrary(extension))
		{
			ret = (*it)->GetLoaderType();
			break;
		}
	}

	return ret;
}

Resource* ResourceManager::Get(std::string unique_id)
{
	return Get(unique_id, ResourceType::RT_NULL);
}

Resource* ResourceManager::Get(std::string unique_id, ResourceType type)
{
	Resource* ret = nullptr;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* curr_loader = (*it);

		if (type == ResourceType::RT_NULL || type == curr_loader->GetLoaderType())
		{
			ret = curr_loader->GetResource(unique_id);

			if (ret != nullptr)
				break;
		}
	}
	return ret;
}

Resource * ResourceManager::CreateNewResource(ResourceType type)
{
	std::string new_id = GetNewUID();
	return CreateNewResource(type, new_id);
}

Resource* ResourceManager::CreateNewResource(ResourceType type, std::string unique_id)
{
	Resource* res = nullptr;

	res = Get(unique_id, type);

	if (res == nullptr)
	{
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			res = loader->CreateResource(unique_id);

			loader->AddResource(res);
		}
	}

	return res;
}

bool ResourceManager::DeleteResource(std::string unique_id)
{
	bool ret = false;

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* loader = (*it);

		if (loader->DeleteResource(unique_id))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void ResourceManager::LoadFileToEngine(const char * filepath, std::vector<Resource*>& resources)
{
	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

		if (type != ResourceType::RT_NULL)
		{
			bool can_load = false;

			std::string new_path;
			if (App->file_system->FileCopyPaste(filepath, App->file_system->GetAssetsPath().c_str(), false, new_path))
			{
				can_load = true;
			}
			else if (deco_file.path == App->file_system->GetAssetsPath())
			{
				new_path = filepath;
				can_load = true;
			}

			if (can_load)
			{
				deco_file = App->file_system->DecomposeFilePath(new_path.c_str());

				ResourceLoader* loader = GetLoader(type);

				if (loader != nullptr)
				{
					bool ret = loader->LoadFileToEngine(deco_file, resources);

					if (ret)
					{
						// SUCCES
					}
					else
					{
						// ERROR
					}
				}
			}
		}
	}
}

void ResourceManager::UnloadAssetFromEngine(const char* filepath)
{
	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			loader->RemoveAssetInfoFromEngine(d_filepath);
		}

		App->file_system->FileDelete(filepath);
	}
}

void ResourceManager::RemoveAssetInfoFromEngine(const char * filepath)
{
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		loader->RemoveAssetInfoFromEngine(d_filepath);
	}
}

void ResourceManager::ClearResourceFromGameObjects(Resource * res)
{
	if (res != nullptr)
	{
		ResourceLoader* load = GetLoader(res->GetType());

		if (load != nullptr)
		{
			std::vector<GameObject*> game_object = App->gameobj->GetListGameObjects();

			for (std::vector<GameObject*>::iterator it = game_object.begin(); it != game_object.end(); ++it)
			{
				load->ClearFromGameObject(res, (*it));
			}
		}
	}
}

void ResourceManager::ExportResourceToLibrary(Resource * resource)
{
	if (resource != nullptr)
	{
		ResourceLoader* loader = GetLoader(resource->GetType());

		if (loader != nullptr)
		{
			bool ret = loader->ExportResourceToLibrary(resource);

			if (ret)
			{
				// SUCCES
			}
			else
			{
				// ERROR
			}
		}
	}
}

void ResourceManager::ImportResourceFromLibrary(const char * filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = LibraryExtensionToType(deco_file.file_extension.c_str());
	
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->ImportResourceFromLibrary(deco_file);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
}

void ResourceManager::LoadAssetIntoScene(const char* filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());
	
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->LoadAssetIntoScene(deco_file);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
}

bool ResourceManager::IsAssetOnLibrary(const char * filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->IsAssetOnLibrary(deco_file, library_files_used);
	}

	return ret;
}

void ResourceManager::ReimportAssetToEngine(const char * filepath)
{
	RemoveAssetInfoFromEngine(filepath);
	LoadFileToEngine(filepath);
}

void ResourceManager::RenameAsset(const char * filepath, const char * new_name)
{
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->RenameAsset(d_filepath, new_name);

		if (ret)
		{
			// SUCCES
		}
		else
		{
			// ERROR
		}
	}
}

bool ResourceManager::IsAssetMeta(const char* filepath, const char* metapath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		if (App->file_system->FileExists(metapath))
		{
			std::string filepath_meta = std::string(filepath) + ".meta";

			if (filepath_meta == std::string(metapath))
			{
				ret = true;
			}
		}
	}

	return ret;
}

bool ResourceManager::FileIsMeta(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		if (deco_file.file_extension == "meta")
			ret = true;
	}

	return ret;
}

std::string ResourceManager::GetNewUID()
{
	return GetUIDRandomHexadecimal();
}

bool ResourceManager::DrawResourceSelector(const char* id, const char * name, ResourceType type, Resource*& res)
{
	bool ret = false;

	std::string res_name = "empty";

	if (res != nullptr)
		res_name = res->GetFileName();

	std::string line_text = std::string(name) + ": ";

	ImGui::PushID(id);

	ImGui::Text(line_text.c_str());
	ImGui::SameLine();

	ImGui::TextColored({ 0.45f, 0.86f, 1, 1.0f }, res_name.c_str());
	ImGui::SameLine();

	if (ImGui::SmallButton("+"))
	{
		ImGui::OpenPopup("HerarchyPopup");
	}

	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	{
		ResourceLoader* loader = GetLoader(type);

		std::map<std::string, Resource*> resources = loader->GetResources();

		if (ImGui::Selectable("Null"))
		{
			res = nullptr;
			draw_resource_selector_win = false;
			ret = true;
		}

		ImGui::Separator();

		for (std::map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if ((*it).second != res)
			{
				ImGui::PushID((*it).second->GetUniqueId().c_str());

				if (ImGui::Selectable((*it).second->GetFileName().c_str()))
				{
					res = (*it).second;
					draw_resource_selector_win = false;
					ret = true;
				}

				ImGui::PopID();
			}

		}

		ImGui::EndPopup();
	}
	

	ImGui::PopID();

	return ret;
}

void ResourceManager::AddLoader(ResourceLoader * loader)
{
	loaders.push_back(loader);
}

CheckCorrectLibraryAsyncTask::CheckCorrectLibraryAsyncTask(AsyncTaskMode mode, uint iterations_per_frame) : AsyncTask(mode, iterations_per_frame, "Checking Assets")
{
}

void CheckCorrectLibraryAsyncTask::Start()
{
	asset_files_to_check = App->file_system->GetFilesInPathAndChilds(App->file_system->GetAssetsPath().c_str());
	asset_files_to_check_count = asset_files_to_check.size();

	check_asset_files = true;
}

void CheckCorrectLibraryAsyncTask::Update()
{
	if (check_asset_files)
		CheckAssetFiles();

	else if (check_asset_meta_files)
		CheckAssetMetaFiles();

	else if (delete_unnecessary)
		DeleteUnnecessary();

	else if (reimport_files)
		ReimportFiles();
}

void CheckCorrectLibraryAsyncTask::Finish()
{
}

void CheckCorrectLibraryAsyncTask::CheckAssetFiles()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - ((float)asset_files_to_check_count / 100.0f) * asset_files_to_check.size();
		progress = progress / 100.0f * 25.0f;
		SetPercentageProgress(progress);
		SetCurrPhase("Checking asset files");

		std::string curr_file = *asset_files_to_check.begin();

		std::vector<std::string> files_to_check;

		bool is_meta = App->resource_manager->FileIsMeta(curr_file.c_str());

		if (is_meta)
		{
			asset_metas_to_check.push_back(curr_file);
			asset_metas_to_check_count = asset_metas_to_check.size();
		}
		else
		{
			bool correct = App->resource_manager->IsAssetOnLibrary(curr_file.c_str(), files_to_check);

			if (correct)
			{
				library_files_used.insert(library_files_used.end(), files_to_check.begin(), files_to_check.end());
			}
			else
			{
				assets_to_reimport.push_back(curr_file);
				assets_to_reimport_count = assets_to_reimport.size();
			}
		}
		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		asset_files_to_check = App->file_system->GetFilesInPathAndChilds(App->file_system->GetAssetsPath().c_str());
		asset_files_to_check_count = asset_files_to_check.size();

		check_asset_files = false;
		check_asset_meta_files = true;
	}
}

void CheckCorrectLibraryAsyncTask::CheckAssetMetaFiles()
{
	if (!asset_metas_to_check.empty())
	{
		float progress = 100 - ((float)asset_metas_to_check_count / 100.0f) * asset_metas_to_check.size();
		progress = progress / 100.0f * 25.0f;
		SetPercentageProgress(25 + progress);
		SetCurrPhase("Checking asset meta files");

		std::string curr_file = *asset_metas_to_check.begin();

		bool used = false;
		for (std::vector<std::string>::iterator it = asset_files_to_check.begin(); it != asset_files_to_check.end();)
		{
			bool is_meta = App->resource_manager->FileIsMeta((*it).c_str());

			if (!is_meta)
			{
				if (App->resource_manager->IsAssetMeta((*it).c_str(), curr_file.c_str()))
				{
					used = true;
					it = asset_files_to_check.erase(it);
					break;
				}
				else
					++it;
			}
			else
			{
				it = asset_files_to_check.erase(it);
			}
		}

		if (!used)
		{
			App->file_system->FileDelete(curr_file.c_str());
		}

		asset_metas_to_check.erase(asset_metas_to_check.begin());
	}
	else
	{
		library_files_to_check = App->file_system->GetFilesInPathAndChilds(App->file_system->GetLibraryPath().c_str());
		library_files_to_check_count = library_files_to_check.size();

		check_asset_meta_files = false;
		delete_unnecessary = true;
	}
}

void CheckCorrectLibraryAsyncTask::DeleteUnnecessary()
{
	if (!library_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)library_files_to_check_count) * library_files_to_check.size();
		progress = (progress / 100.0f) * 25.0f;
		SetPercentageProgress(50 + progress);
		SetCurrPhase("Deleting library garbage");

		std::string curr_file = *library_files_to_check.begin();

		bool to_delete = true;
		for (std::vector<std::string>::iterator it = library_files_used.begin(); it != library_files_used.end(); ++it)
		{
			if ((*it) == curr_file)
			{
				library_files_used.erase(it);
				to_delete = false;
				break;
			}
		}

		if (to_delete)
		{
			App->file_system->FileDelete(curr_file.c_str());
		}

		library_files_to_check.erase(library_files_to_check.begin());
	}
	else
	{
		delete_unnecessary = false;
		reimport_files = true;
	}
}

void CheckCorrectLibraryAsyncTask::ReimportFiles()
{
	if (!assets_to_reimport.empty())
	{
		float progress = 100 - (100.0f / (float)assets_to_reimport_count) * assets_to_reimport.size();
		progress = (progress / 100.0f) * 25.0f;
		SetPercentageProgress(75 + progress);
		SetCurrPhase("Reimporting files");

		std::string curr_file = *assets_to_reimport.begin();

		App->resource_manager->LoadFileToEngine(curr_file.c_str());

		assets_to_reimport.erase(assets_to_reimport.begin());
	}
	else
	{
		SetPercentageProgress(100);

		FinishTask();
	}
}

LoadLibraryAsyncTask::LoadLibraryAsyncTask(AsyncTaskMode mode, uint iterations_per_frame) : AsyncTask(mode, iterations_per_frame, "Loading Resources")
{
}

void LoadLibraryAsyncTask::Start()
{
	library_files_to_load = App->file_system->GetFilesInPathAndChilds(App->file_system->GetLibraryPath().c_str());
	library_files_to_load_count = library_files_to_load.size();
}

void LoadLibraryAsyncTask::Update()
{
	if (!library_files_to_load.empty())
	{
		float progress = 100 - (100.0f / (float)library_files_to_load_count) * library_files_to_load.size();
		progress = (progress / 100.0f) * 100.0f;
		SetPercentageProgress(progress);
		SetCurrPhase("Loading Library");

		std::string curr_file = *library_files_to_load.begin();

		App->resource_manager->ImportResourceFromLibrary(curr_file.c_str());

		library_files_to_load.erase(library_files_to_load.begin());
	}
	else
	{
		SetPercentageProgress(100);
		FinishTask();
	}
}

void LoadLibraryAsyncTask::Finish()
{
}
