#include "ResourcePrefab.h"

ResourcePrefab::ResourcePrefab(std::string unique_id) : Resource(unique_id, ResourceType::RT_PREFAB)
{
}

ResourcePrefab::~ResourcePrefab()
{
}

void ResourcePrefab::CleanUp()
{
}

void ResourcePrefab::SetAbstraction(const GameObjectAbstraction & _abstraction)
{
	abstraction = _abstraction;
}

GameObjectAbstraction ResourcePrefab::GetAbstraction() const
{
	return abstraction;
}
