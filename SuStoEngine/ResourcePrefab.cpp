#include "ResourcePrefab.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "GameObjectAbstractor.h"

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

GameObjectAbstraction ResourcePrefab::GetAbstraction() const
{
	return abstraction;
}
