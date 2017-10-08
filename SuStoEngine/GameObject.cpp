#include "GameObject.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentPrimitive.h"
#include "ComponentTransform.h"
#include "ModuleGameObject.h"
#include "Inspector.h"

GameObject::GameObject()
{
	LOG_OUTPUT("Game Object Created");
}

GameObject::~GameObject()
{
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
	case TRANSFORM:
	{
		tmp = new ComponentTransfrom(this);		
	}
		break;
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

std::list<Component*> GameObject::GetComponents()
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

int GameObject::GetId()
{
	return id;
}

void GameObject::SetId(int _id)
{
	id = _id;
	sprintf_s(name, 25, "GameObject_%d", id);
}

void GameObject::HierarchyView()
{
	bool tmp = App->gameobj->selected_go[id];
	ImGui::Selectable(name, &App->gameobj->selected_go[id]);

	if (tmp != App->gameobj->selected_go[id])
	{
		for (int i = 0; i < 250; ++i) 
		{
			if (i != id)
				App->gameobj->selected_go[i] = App->gameobj->all_false[i];
		}
		App->editorUI->GetInspector()->is_selected = true;
	}
}

