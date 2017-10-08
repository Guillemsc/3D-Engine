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
	switch (type)
	{
	case TRANSFORM:
	{
		ComponentTransfrom* c_transform = new ComponentTransfrom(this);
		components.push_back(c_transform);
	}
		break;
	case PRIMITIVE:
	{
		ComponentPrimitive* c_primitive = new ComponentPrimitive(this);
		components.push_back(c_primitive);
	}
		break;
	default:
		break;
	}
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

int GameObject::GetId()
{
	return id;
}

void GameObject::SetId(int _id)
{
	id = _id;
}

void GameObject::HierarchyView()
{
	char name[25];
	sprintf_s(name, 25, "GameObject_%d", id);
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

