#include "ResourceManager.h"
#include "Functions.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "App.h"
#include "GeometryLoader.h"
#include "ModuleFileSystem.h"
#include "TextureLoader.h"

ResourceManager::ResourceManager(bool start_enabled)
{
}

ResourceManager::~ResourceManager()
{
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
		App->geometry->GetMeshImporter()->Save(App->file_system->GetLibraryMeshPath().c_str(), (ResourceMesh*)res);
		break;
	case ResourceType::RT_TEXTURE:
		App->texture->GetTextureImporter()->Save(App->file_system->GetLibraryTexturePath().c_str(), (ResourceTexture*)res);
		break;
	case ResourceType::RT_SCENE:
		break;
	}
}

Resource * ResourceManager::LoadResource(const char * file_path)
{
	return nullptr;
}
