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

Resource * ResourceManager::Get(double id)
{
	std::map<double, Resource*>::iterator it = resources.find(id);

	if (it != resources.end())
		return it->second;

	return nullptr;
}

Resource * ResourceManager::CreateNewResource(ResourceType type, double force_id)
{
	double new_id = 0;

	if (force_id == -1)
		new_id = GetUniqueIdentifierRandom();
	else
		new_id = force_id;

	Resource* res = Get(new_id);

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

void ResourceManager::DeleteResource(double id)
{
	std::map<double, Resource*>::iterator it = resources.find(id);

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
