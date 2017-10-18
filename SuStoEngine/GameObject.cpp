#include "GameObject.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentPrimitive.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleGameObject.h"
#include "TextureLoader.h"
#include "Inspector.h"
#include "ModuleRenderer3D.h"


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
	transform = (ComponentTransform*)FindComponentByType(TRANSFORM);
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
	App->renderer3D->DrawAxis(transform->GetPosition(), transform->GetRotationEuler());

	// Push matrix
	glPushMatrix();
	glMultMatrixf(transform->GetTransform().ptr());

	ComponentMaterial* component_material = (ComponentMaterial*)FindComponentByType(MATERIAL);

	if (component_material != nullptr)
	{
		if (component_material->HasTexture())
		{
			glBindTexture(GL_TEXTURE_2D, component_material->GetTexture()->GetId());
		}
	}

	ComponentMesh* component_mesh = (ComponentMesh*)FindComponentByType(MESH);
	
	if (component_mesh != nullptr)
	{
		if (component_mesh->HasMesh())
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
	}
	
	// Reset
	glBindTexture(GL_TEXTURE_2D, 0);

	// Pop matrix
	glPopMatrix();
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
			ret = new ComponentTransform(this);
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
		case MATERIAL:
		{
			ret = new ComponentMaterial(this);
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
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == type)
		{
			(*it)->CleanUp();
			components.erase(it);
			break;
		}
	}
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

const GameObject* GameObject::GetParent() const
{
	return parent;
}

GameObject* GameObject::GetParent()
{
	return parent;
}

const std::vector<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

const uint GameObject::GetChildsCount() const
{
	return childs.size();
}

const void GameObject::SetParentToNull()
{
	parent = nullptr;
}

void GameObject::EraseChild(GameObject * child, bool send_child_to_root)
{
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{		
		if ((*it) == child)
		{
			// Clean
			child->SetParentToNull();
			childs.erase(it);

			// Add to root
			if(send_child_to_root)
				App->gameobj->GetRoot()->AddChild(child);

			break;
		}
	}
}

void GameObject::AddChild(GameObject * child)
{
	if (child == nullptr)
		return;

	// Clean from parent
	if (child->parent != nullptr)
		child->parent->EraseChild(child, false);
	
	// Add new parent
	child->parent = this;
	childs.push_back(child);
}


