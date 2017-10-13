#include "GameObject.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentPrimitive.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentTexture.h"
#include "ModuleGameObject.h"
#include "TextureLoader.h"
#include "Inspector.h"

#include "Glew/include/glew.h" 

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

void GameObject::Draw()
{
	ComponentTexture* component_texture = (ComponentTexture*)FindComponentByType(TEXTURE);

	if (component_texture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, component_texture->GetTexture()->GetId());
	}

	ComponentMesh* component_mesh = (ComponentMesh*)FindComponentByType(MESH);
	
	if (component_mesh != nullptr)
	{
		// Vertex
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdVertices());
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		// UV
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdUV());
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		// Index
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdIndices());

		// Draw
		glDrawElements((GLenum)GL_TRIANGLES, component_mesh->GetMesh()->GetNumIndices(), GL_UNSIGNED_INT, NULL);

		// Disable
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
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

const bool GameObject::GetEnabled() const
{
	return enabled;
}

void GameObject::SetEnabled(const bool& set)
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

const std::vector<Component*> GameObject::GetComponents() const
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

void GameObject::SetName(const string& set)
{
	name = set;
}

void GameObject::SetSelected(const bool& set)
{
	selected = set;
}

const int GameObject::GetId() const
{
	return id;
}

const bool GameObject::GetSelected() const
{
	return selected;
}


