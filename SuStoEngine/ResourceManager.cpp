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

ResourceManager::ResourceManager(bool start_enabled)
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

	AddLoader(mesh_loader);
	AddLoader(texture_loader);

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

void ResourceManager::OnLoadFile(const char * filepath)
{
	LoadResourceToEngine(filepath);
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

void ResourceManager::LoadResourceToEngine(const char * filepath)
{
	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

	if (type != ResourceType::RT_NULL)
	{
		std::string new_path;
		if (App->file_system->FileCopyPaste(filepath, App->file_system->GetAssetsPath().c_str(), new_path))
		{
			deco_file = App->file_system->DecomposeFilePath(new_path.c_str());

			ResourceLoader* loader = GetLoader(type);

			if (loader != nullptr)
			{
				std::vector<Resource*> resources;

				bool ret = loader->LoadToEngine(deco_file, resources);

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

void ResourceManager::UnloadResourceFromEngine(const char* filepath)
{
	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		loader->UnloadFromEngine(d_filepath);
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
			bool ret = loader->ExportToLibrary(resource);

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
		bool ret = loader->ImportFromLibrary(deco_file.file_name.c_str());

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

bool ResourceManager::IsResourceOnLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourceLoader* loader = GetLoader(resource->GetType());

		if (loader != nullptr)
		{
			ret = loader->IsResourceOnLibrary(resource);
		}
	}

	return ret;
}

bool ResourceManager::IsResourceOnAssets(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourceLoader* loader = GetLoader(resource->GetType());

		if (loader != nullptr)
		{
			ret = loader->IsResourceOnAssets(resource);
		}
	}

	return ret;
}

void ResourceManager::CreateResourcesMissingOnAssets()
{
	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it)->CreateResourcesMissingOnAssets();
	}
}

void ResourceManager::RemoveResourcesMissingOnLibrary()
{
	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it)->RemoveResourcesMissingOnLibrary();
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

//void ResourceManager::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
//{
//	LoadResource(file_path);
//}
//
//void ResourceManager::DeleteAllResources()
//{
//	for (map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end();)
//	{
//		(*it).second->CleanUp();
//		RELEASE(it->second);
//		it = resources.erase(it);
//	}
//}
