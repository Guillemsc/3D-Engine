#include "ResourcePrefab.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "GameObjectAbstractor.h"
#include "ModuleFileSystem.h"

ResourcePrefab::ResourcePrefab(std::string unique_id) : Resource(unique_id, ResourceType::RT_PREFAB)
{
}

ResourcePrefab::~ResourcePrefab()
{
}

void ResourcePrefab::CleanUp()
{
}

void ResourcePrefab::SetGameObject(GameObject * go)
{
	abstraction = App->gameobj->GetAbstractor()->Abstract(go);
}

void ResourcePrefab::SetAbstraction(const GameObjectAbstraction& abs)
{
	abstraction = abs;
}

void ResourcePrefab::Instantiate(GameObject * parent)
{
	if (abstraction.GetValid())
	{
		GameObject* instance = App->gameobj->GetAbstractor()->DeAbstract(abstraction);

		if (instance != nullptr)
		{
			if (parent != nullptr)
			{
				instance->SetParent(parent);
			}
		}
	}
}

void ResourcePrefab::Serialize()
{
	if (abstraction.GetValid())
	{
		App->gameobj->GetAbstractor()->Serialize(abstraction, App->file_system->GetLibraryPrefabPath().c_str(), abstraction.GetName().c_str(), "prefab");
	}
}

void ResourcePrefab::SerializeCustom(const char * path, const char * name, const char * extension)
{
	if (abstraction.GetValid())
	{
		App->gameobj->GetAbstractor()->Serialize(abstraction, path, name, extension);
	}
}

GameObjectAbstraction ResourcePrefab::GetAbstraction() const
{
	return abstraction;
}
