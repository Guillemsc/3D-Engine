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

ResourceManager::ResourceManager(bool start_enabled)
{
	mesh_loader = new ResourceMeshLoader();
	texture_loader = new ResourceTextureLoader();
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Start()
{
	bool ret = true;

	return ret;
}

bool ResourceManager::CleanUp()
{
	bool ret = true;

	DeleteAllResources();

	RELEASE(mesh_loader);
	RELEASE(texture_loader);

	return ret;
}

Resource * ResourceManager::Get(std::string _unique_id)
{
	std::map<std::string, Resource*>::iterator it = resources.find(_unique_id);

	if (it != resources.end())
		return it->second;

	return nullptr;
}

Resource * ResourceManager::CreateNewResource(ResourceType type, std::string _unique_id)
{
	std::string new_id;

	if (_unique_id == "")
		new_id = GetNewUID();
	else
		new_id = _unique_id;

	Resource* res = Get(_unique_id);

	if (res == nullptr)
	{
		switch (type)
		{
		case ResourceType::RT_MESH:
			res = new ResourceMesh(new_id);
			break;
		case ResourceType::RT_TEXTURE:
			res = new ResourceTexture(new_id);
			break;
		case ResourceType::RT_SCENE:
			break;
		}

		if (res != nullptr)
		{
			resources[new_id] = res;
		}
	}

	return res;
}

void ResourceManager::DeleteResource(std::string unique_id)
{
	std::map<std::string, Resource*>::iterator it = resources.find(unique_id);

	if (it != resources.end())
	{
		it->second->CleanUp();
		RELEASE(it->second);
		resources.erase(it);
	}
}

void ResourceManager::SaveResourceIntoFile(Resource * res)
{
	if (res != nullptr)
	{
		switch (res->GetType())
		{
		case ResourceType::RT_MESH:
			mesh_loader->Export(App->file_system->GetLibraryMeshPath().c_str(), (ResourceMesh*)res);
			break;
		case ResourceType::RT_TEXTURE:
			texture_loader->Export(App->file_system->GetLibraryTexturePath().c_str(), (ResourceTexture*)res);
			break;
		case ResourceType::RT_SCENE:
			break;
		}
	}
}

bool ResourceManager::LoadResource(const char * file_path)
{
	vector<Resource*> resources;
	return LoadResource(file_path, resources);
}

bool ResourceManager::LoadResource(const char * file_path, vector<Resource*>& resources)
{
	bool ret = false;

	resources.clear();

	string name = App->file_system->GetFileNameFromFilePath(file_path);
	string extension = ToLowerCase(App->file_system->GetFileExtension(name.c_str()));

	bool valid_extension = false;

	if (TextCmp("fbx", extension.c_str()))
	{
		ret = mesh_loader->Load(file_path, resources, true);
		valid_extension = true;
	}
	else if (TextCmp("png", extension.c_str()) || TextCmp("dds", extension.c_str()) || TextCmp("tga", extension.c_str()))
	{
		ret = texture_loader->Load(file_path, resources);
	}

	if (ret)
	{
		App->file_system->FileCopyPaste(file_path, App->file_system->GetAssetsPath().c_str());
		string uid = GetNewUID();
		string json_name = App->file_system->GetAssetsPath() + name + ".meta";
		JSON_Doc* meta = App->json->CreateJSON(json_name.c_str());
		meta->SetString("uid", uid.c_str());

		meta->SetArray("resources");
		for (vector<Resource*>::iterator res = resources.begin(); res != resources.end(); ++res)
		{
			meta->AddStringToArray("resources", (*res)->GetUniqueId().c_str());
			(*res)->SetOriginalFileUID(uid.c_str());
		}

		meta->Save();
	}

	return ret;
}

void ResourceManager::ImportAllResources()
{
	mesh_loader->ImportAllMeshes();
	texture_loader->ImportAllTextures();
}

ResourceMeshLoader * ResourceManager::GetMeshLoader()
{
	return mesh_loader;
}

ResourceTextureLoader * ResourceManager::GetTextureLoader()
{
	return texture_loader;
}

std::string ResourceManager::GetNewUID()
{
	return GetUIDRandomHexadecimal();
}

void ResourceManager::OnLoadFile(const char * file_path, const char * file_name, const char * file_extension)
{
	LoadResource(file_path);
}

void ResourceManager::DeleteAllResources()
{
	for (map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end();)
	{
		(*it).second->CleanUp();
		RELEASE(it->second);
		it = resources.erase(it);
	}
}
