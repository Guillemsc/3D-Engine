#include "GameObject.h"
#include "Functions.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ModuleGameObject.h"
#include "TextureLoader.h"
#include "Inspector.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "JSONLoader.h"

#include "Glew/include/glew.h" 

GameObject::GameObject(std::string _unique_id)
{
	unique_id = _unique_id;
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	LOG_OUTPUT("Game Object Created");

	SetName("GameObject");

	AddComponent(TRANSFORM);
	transform = (ComponentTransform*)GetComponent(TRANSFORM);
}

void GameObject::Update()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if((*it)->GetEnabled())
			(*it)->Update();
	}
}

void GameObject::Draw()
{
	// Push matrix
	glPushMatrix();
	glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());

	ComponentMaterial* component_material = (ComponentMaterial*)GetComponent(MATERIAL);

	if (component_material != nullptr && component_material->GetEnabled())
	{
		if (component_material->HasTexture())
		{
			glBindTexture(GL_TEXTURE_2D, component_material->GetTexture()->GetTextureId());
		}
	}

	ComponentMesh* component_mesh = (ComponentMesh*)GetComponent(MESH);
	
	if (component_mesh != nullptr && component_mesh->GetEnabled())
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

	// Pop matrix
	glPopMatrix();

	DrawBBox();
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

void GameObject::AddComponent(ComponentType type, string unique_id)
{
	if (ContainsComponent(type))
		return;

	string new_id;

	if (new_id == "")
		new_id = GetUIDRandomHexadecimal();
	else
		new_id = unique_id;

	Component* ret = nullptr;

	switch (type)
	{
		case TRANSFORM:
		{
			ret = new ComponentTransform(this, new_id);
		}
		break;
		case MESH:
		{
			ret = new ComponentMesh(this, new_id);
		}
		break;
		case MATERIAL:
		{
			ret = new ComponentMaterial(this, new_id);
		}
		break;
		case CAMERA:
		{
			ret = new ComponentCamera(this, new_id);
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

std::vector<Component*> GameObject::GetComponents()
{
	return components;
}

Component * GameObject::GetComponent(ComponentType type)
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

std::string GameObject::GetUniqueId()
{
	return unique_id;
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
			if (send_child_to_root)
			{
				App->gameobj->GetRoot()->AddChild(child);

				child->transform->local_transform = child->transform->global_transform;
			}

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

void GameObject::RecursiveCalcGlobalTransform()
{
	if (parent != nullptr)
	{
		transform->global_transform = parent->transform->global_transform * transform->local_transform;
	}

	if (!childs.empty())
	{
		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->RecursiveCalcGlobalTransform();
		}
	}
}

void GameObject::RecursiveCalcBBox()
{
	local_bbox.SetNegativeInfinity();

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
		(*it)->OnGetBoundingBox(local_bbox);

	if (local_bbox.IsFinite())
	{
		local_bbox.TransformAsAABB(transform->GetGlobalTransform());
		//global_bbox = local_bbox;
		//global_bbox.Transform(transform->GetGlobalTransform());
		//global_bbox = global_bbox.MinimalEnclosingAABB();
	}
	
	if (!childs.empty())
	{
		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->RecursiveCalcBBox();
		}
	}
}

bool GameObject::GetStatic()
{
	return is_static;
}

void GameObject::SetStatic(bool set)
{
	is_static = set;
}

AABB GameObject::GetBbox() const
{
	return local_bbox;
}

void GameObject::OnLoadScene(JSON_Doc * config)
{
	config->MoveToRoot();

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		(*it)->OnLoadScene(config);
	}
}

void GameObject::OnSaveScene(JSON_Doc * config)
{
	// Add and move to a new secion on the gameobjects array
	config->AddSectionToArray("GameObjects");
	config->MoveToSectionFromArray("GameObjects", config->GetArrayCount("GameObjects") - 1);

	// Set the id
	config->SetString("uid", unique_id.c_str());
	
	// Set the name
	config->SetString("name", name.c_str());

	// Set the parent id
	if (parent != nullptr && parent != App->gameobj->GetRoot())
		config->SetString("parent", parent->GetUniqueId().c_str());
	else
		config->SetNumber("parent", -1);

	config->MoveToRoot();

	// Save components
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		// Add and move to a new section on the components array
		config->AddSectionToArray("Components");
		config->MoveToSectionFromArray("Components", config->GetArrayCount("Components") - 1);

		config->SetNumber("type", (*it)->GetType());
		config->SetString("component_id", (*it)->GetUniqueId().c_str());
		config->SetString("owner_id", (*it)->GetOwner()->GetUniqueId().c_str());

		(*it)->OnSaveScene(config);

		config->MoveToRoot();
	}
}

void GameObject::DrawBBox()
{
	if (local_bbox.IsFinite())
		DebugDraw(local_bbox, White, true, 4.0f);
}


