#include "GameObject.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentPrimitive.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
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

	AddComponent(TRANSFORM);
	transform = FindComponentByType(TRANSFORM);
}

void GameObject::Update()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->Update();
	}
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
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it)
	{
		(*it)->CleanUp();
		delete(*it);
		it = components.erase(it);
	}
}

void GameObject::AddComponent(ComponentType type)
{
	if (ContainsComponent(type))
		return;

	Component* ret = nullptr;

	switch (type)
	{
		case TRANSFORM:
		{
			ret = new ComponentTransfrom(this);
		}
		break;
		case MESH:
		{
			ret = new ComponentMesh(this);
		}
		break;
		case PRIMITIVE:
		{
			ret = new ComponentPrimitive(this);
		}
		break;
		case TEXTURE:
		{
			ret = new ComponentTexture(this);
		}
		break;
	default:
		break;
	}
	
	if (ret != nullptr)
	{
		ret->Start();
		components.push_back(ret);
	}
}

void GameObject::RemoveComponent(ComponentType type)
{
}

bool GameObject::ContainsComponent(ComponentType type)
{
	bool ret = false;

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == type)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}

Component * GameObject::FindComponentByType(ComponentType type)
{
	Component* ret = nullptr;

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == type)
		{
			ret = *it;
			break;
		}
	}

	return ret;
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

void GameObject::SetName(string set)
{
	name = set;
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


