#include "GameObject.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentPrimitive.h"
#include "ComponentTransform.h"
#include "ModuleGameObject.h"
#include "Inspector.h"

GameObject::GameObject(int _id)
{
	id = _id;
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	LOG_OUTPUT("Game Object Created");

	SetName("GameObject");

	ComponentTransfrom* trans = new ComponentTransfrom(this);
	components.push_back(trans);
}

void GameObject::Update()
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
	Component* tmp = nullptr;

	switch (type)
	{
		case PRIMITIVE:
		{
			tmp = new ComponentPrimitive(this);
		}
		break;

	default:
		break;
	}
	
	if (tmp != nullptr)
		components.push_back(tmp);
}

void GameObject::RemoveComponent(ComponentType type)
{
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}

Component * GameObject::FindComponentByType(ComponentType type)
{
	return nullptr;
}

const char * GameObject::GetName() const
{
	return name;
}

void GameObject::SetName(const char * new_name)
{
	sprintf_s(name, 25, new_name);
}

void GameObject::SetSelected(bool set)
{
	selected = set;
}

int GameObject::GetId()
{
	return id;
}

bool GameObject::GetSelected()
{
	return selected;
}


