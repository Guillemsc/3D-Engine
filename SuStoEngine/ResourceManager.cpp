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
	mesh_loader->AddAssetExtensionToLoad("png");
	mesh_loader->AddAssetExtensionToLoad("tga");
	mesh_loader->AddAssetExtensionToLoad("dds");
	mesh_loader->AddLibraryExtensionToLoad("dds");

	ResourcePrefabLoader* prefab_loader = new ResourcePrefabLoader();
	prefab_loader->AddAssetExtensionToLoad("prefab");
	prefab_loader->AddLibraryExtensionToLoad("prefab");
	prefab_loader->AddLibraryExtensionToLoad("meshprefab");

	AddLoader(mesh_loader);
	AddLoader(texture_loader);
	AddLoader(prefab_loader);

	OnStartEngine();

	return ret;
}

bool ResourceManager::Start()
{
	bool ret = true;

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
	CheckCorrectLibraryAsyncTaks* task = new CheckCorrectLibraryAsyncTaks(AsyncTaskMode::AST_FOCUS, 2);
	App->async_tasks->StartTask(task);
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

	res = Get(unique_id);

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

void ResourceManager::LoadFileToEngine(const char * filepath)
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
					std::vector<Resource*> resources;

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
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		loader->UnloadAssetFromEngine(d_filepath);
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
		bool ret = loader->ImportResourceFromLibrary(deco_file.file_name.c_str());

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

void ResourceManager::LoadAssetResourceIntoScene(const char* filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());
	
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		bool ret = loader->LoadAssetResourceIntoScene(deco_file);

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

//void ResourceManager::SaveResourceIntoFile(Resource * res)
//{
//	if (res != nullptr)
//	{
//		switch (res->GetType())
//		{
//		case ResourceType::RT_MESH:
//			mesh_loader->Export(App->file_system->GetLibraryMeshPath().c_str(), (ResourceMesh*)res);
//			break;
//		case ResourceType::RT_TEXTURE:
//			texture_loader->Export(App->file_system->GetLibraryTexturePath().c_str(), (ResourceTexture*)res);
//			break;
//		case ResourceType::RT_SCENE:
//			break;
//		}
//	}
//}
//
//bool ResourceManager::LoadResource(const char * file_path)
//{
//	vector<Resource*> resources;
//	return LoadResource(file_path, resources);
//}
//
//bool ResourceManager::LoadResource(const char * file_path, vector<Resource*>& resources)
//{
//	bool ret = false;
//
//	resources.clear();
//
//	string name = App->file_system->GetFileNameFromFilePath(file_path);
//	string extension = ToLowerCase(App->file_system->GetFileExtension(name.c_str()));
//
//	bool valid_extension = false;
//
//	if (TextCmp("fbx", extension.c_str()))
//	{
//		ret = mesh_loader->Load(file_path, resources, true);
//		valid_extension = true;
//	}
//	else if (TextCmp("png", extension.c_str()) || TextCmp("dds", extension.c_str()) || TextCmp("tga", extension.c_str()))
//	{
//		ret = texture_loader->Load(file_path, resources);
//	}
//
//	if (ret)
//	{
//		App->file_system->FileCopyPaste(file_path, App->file_system->GetAssetsPath().c_str());
//		
//		// Save meta file ---------------------------
//		string uid = GetNewUID();
//		string json_name = App->file_system->GetAssetsPath() + name + ".meta";
//		JSON_Doc* meta = App->json->CreateJSON(json_name.c_str());
//		if (meta)
//		{
//			meta->SetString("uid", uid.c_str());
//
//			meta->SetArray("resources");
//			for (vector<Resource*>::iterator res = resources.begin(); res != resources.end(); ++res)
//			{
//				meta->AddStringToArray("resources", (*res)->GetUniqueId().c_str());
//			}
//
//			meta->Save();
//		}
//
//	}
//
//	return ret;
//}
//
//void ResourceManager::ImportAllResources()
//{
//	mesh_loader->ImportAllMeshes();
//	texture_loader->ImportAllTextures();
//}
//
//void ResourceManager::LoadFileIntoScene(const char * file_path)
//{
//	string name = App->file_system->GetFileNameFromFilePath(file_path);
//	string extension = ToLowerCase(App->file_system->GetFileExtension(name.c_str()));
//
//	if (TextCmp("fbx", extension.c_str()))
//	{
//		mesh_loader->LoadIntoScene(file_path);
//	}
//	else if (TextCmp("png", extension.c_str()) || TextCmp("dds", extension.c_str()) || TextCmp("tga", extension.c_str()))
//	{
//
//	}
//}
//
//void ResourceManager::DeImportFile(const char * file_path)
//{
//	string name = App->file_system->GetFileNameFromFilePath(file_path);
//	string extension = ToLowerCase(App->file_system->GetFileExtension(name.c_str()));
//
//	if (TextCmp("fbx", extension.c_str()))
//	{
//		mesh_loader->Unload(file_path);
//	}
//	else if (TextCmp("png", extension.c_str()) || TextCmp("dds", extension.c_str()) || TextCmp("tga", extension.c_str()))
//	{
//		texture_loader->Unload(file_path);
//	}
//}

std::string ResourceManager::GetNewUID()
{
	return GetUIDRandomHexadecimal();
}

void ResourceManager::AddLoader(ResourceLoader * loader)
{
	loaders.push_back(loader);
}

CheckCorrectLibraryAsyncTaks::CheckCorrectLibraryAsyncTaks(AsyncTaskMode mode, uint iterations_per_frame) : AsyncTask(mode, iterations_per_frame, "Loading Resources")
{
}

void CheckCorrectLibraryAsyncTaks::Start()
{
	asset_files_to_check = App->file_system->GetFilesInPathAndChilds(App->file_system->GetAssetsPath().c_str());
	asset_files_to_check_count = asset_files_to_check.size();

	check_asset_files = true;
}

void CheckCorrectLibraryAsyncTaks::Update()
{
	if (check_asset_files)
		CheckAssetFiles();

	else if (delete_unnecessary)
		DeleteUnnecessary();

	else if (reimport_files)
		ReimportFiles();
}

void CheckCorrectLibraryAsyncTaks::Finish()
{
}

void CheckCorrectLibraryAsyncTaks::CheckAssetFiles()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - ((float)asset_files_to_check_count / 100.0f) * asset_files_to_check.size();
		progress = progress / 100.0f * 33.0f;
		SetPercentageProgress(progress);
		SetCurrPhase("Checking asset files");

		std::string curr_file = *asset_files_to_check.begin();

		std::vector<std::string> files_to_check;
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

		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		library_files_to_check = App->file_system->GetFilesInPathAndChilds(App->file_system->GetLibraryPath().c_str());
		library_files_to_check_count = library_files_to_check.size();

		check_asset_files = false;
		delete_unnecessary = true;
	}
}

void CheckCorrectLibraryAsyncTaks::DeleteUnnecessary()
{
	if (!library_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)library_files_to_check_count) * library_files_to_check.size();
		progress = (progress / 100.0f) * 33.0f;
		SetPercentageProgress(33 + progress);
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

void CheckCorrectLibraryAsyncTaks::ReimportFiles()
{
	if (!assets_to_reimport.empty())
	{
		float progress = 100 - (100.0f / (float)assets_to_reimport_count) * assets_to_reimport.size();
		progress = (progress / 100.0f) * 33.0f;
		SetPercentageProgress(66 + progress);
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
