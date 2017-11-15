#include "ResourceManager.h"
#include "Functions.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ResourceMeshLoader.h"
#include "ResourceTextureLoader.h"
#include "Globals.h"

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
		new_id = GetUIDRandomHexadecimal();
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

Resource * ResourceManager::LoadResource(const char * file_path)
{
	Resource* ret = nullptr;

	string path = ProcessFilePath(file_path);
	string name = GetFileNameFromFilePath(file_path);
	string extension = ToLowerCase(GetFileExtension(name.c_str()));

	if (TextCmp("fbx", extension.c_str()))
	{
		mesh_loader->Load(file_path, true);
	}
	else if (TextCmp("png", extension.c_str()) || TextCmp("dds", extension.c_str()) || TextCmp("tga", extension.c_str()))
	{
		ret = texture_loader->Load(file_path);
	}

	return ret;
}

void ResourceManager::ImportAllResources()
{
	mesh_loader->ImportAllMeshes();
	texture_loader->ImportAllTextures();
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
