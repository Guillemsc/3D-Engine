#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
void GameObject::Enable()
{
	if (!enabled)
	{
		enabled = true; 
	}
}

void GameObject::Disable()
{
	if (enabled)
	{
		enabled = true;
	}
}

bool GameObject::GetEnabled()
{
	return enabled;
}

void GameObject::SetEnabled(bool set)
{
	set ? Enable() : Disable();
}

void GameObject::CleanUp()
{
}

void GameObject::AddComponent(ComponentType type)
{
}

void GameObject::RemoveComponent(ComponentType type)
{
}

std::list<Component*> GameObject::GetComponents()
{
	return std::list<Component*>();
}

Component * GameObject::FindComponentByType(ComponentType type)
{
	return nullptr;
}
