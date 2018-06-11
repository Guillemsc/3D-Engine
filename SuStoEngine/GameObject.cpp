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
#include "Inspector.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "JSONLoader.h"
#include "ResourceManager.h"
#include "ModuleEventSystem.h"

#include "Glew/include/glew.h" 

GameObject::GameObject(std::string _unique_id, ModuleGameObject* _go_module)
{
	unique_id = _unique_id;
	go_module = _go_module;
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	CONSOLE_LOG("Game Object Created");

	SetName("GameObject");

	transform = (ComponentTransform*)AddComponent(TRANSFORM);
}

void GameObject::PreUpdate()
{
}

void GameObject::UpdateComponents()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if((*it)->GetActive())
			(*it)->Update();
	}
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
}

void GameObject::Draw()
{
	if (!draw)
		return;

	// Push matrix
	glPushMatrix();
	glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());

	ComponentMaterial* component_material = (ComponentMaterial*)GetComponent(MATERIAL);

	if (component_material != nullptr && component_material->GetActive())
	{
		if (component_material->HasTexture())
		{
			glBindTexture(GL_TEXTURE_2D, component_material->GetTexture()->GetTextureId());
		}
	}

	ComponentMesh* component_mesh = (ComponentMesh*)GetComponent(MESH);

	if (component_mesh != nullptr && component_mesh->GetActive())
	{
		if (component_mesh->HasMesh())
		{
			// Vertex
			glEnableClientState(GL_VERTEX_ARRAY);

			if (component_mesh->GetMesh()->GetIdVertices() != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdVertices());
				glVertexPointer(3, GL_FLOAT, 0, NULL);

				if (component_mesh->GetMesh()->GetIdUV() != 0)
				{
					// UV
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdUV());
					glTexCoordPointer(3, GL_FLOAT, 0, NULL);
				}

				if (component_mesh->GetMesh()->GetIdIndices() != 0)
				{
					// Index
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component_mesh->GetMesh()->GetIdIndices());

					// Draw
					glDrawElements((GLenum)GL_TRIANGLES, component_mesh->GetMesh()->GetNumIndices(), GL_UNSIGNED_INT, NULL);
				}
			}

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

	DrawBBox();
}

void GameObject::CleanUp()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		(*it)->CleanUp();
		RELEASE(*it);
		it = components.erase(it);
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

const bool GameObject::GetEnabled() const
{
	return enabled;
}

void GameObject::SetEnabled(const bool& set)
{
	set ? Enable() : Disable();
}

Component* GameObject::AddComponent(ComponentType type, string unique_id)
{
	Component* ret = nullptr;

	string new_id;

	if (new_id == "")
		new_id = App->resource_manager->GetNewUID();
	else
		new_id = unique_id;

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
	}
	
	if (ret != nullptr)
	{
		if (ret->GetOnePerGo())
		{
			if (ContainsComponent(type))
			{
				ret->CleanUp();
				delete ret;
				ret = nullptr;
			}
		}

		if (ret != nullptr)
		{
			ret->Start();
			components.push_back(ret);

			Event ev(EventType::ET_COMPONENT_CREATE);
			ev.component_create.component = ret;
			App->event_system->Send(ev);
		}
	}

	return ret;
}

bool GameObject::RemoveComponent(ComponentType type)
{
	bool ret = false;

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == type)
		{
			if ((*it)->GetCanDestroy())
			{
				for (std::list<Component*>::iterator des = components_to_destroy.begin(); des != components_to_destroy.end(); ++des)
				{
					if ((*des) == (*it))
					{
						return false;
					}
				}

				Event ev(EventType::ET_COMPONENT_CREATE);
				ev.component_create.component = (*it);
				App->event_system->Send(ev);

				components_to_destroy.push_back(*it);

				ret = true;
			}

			break;
		}
	}

	return ret;
}

bool GameObject::RemoveComponent(Component * comp)
{
	bool ret = false;

	if (comp != nullptr)
	{
		if (comp->GetCanDestroy())
		{
			for (std::list<Component*>::iterator des = components_to_destroy.begin(); des != components_to_destroy.end(); ++des)
			{
				if ((*des) == comp)
				{
					return false;
				}
			}

			for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
			{
				if ((*it) == comp)
				{
					Event ev(EventType::ET_COMPONENT_CREATE);
					ev.component_create.component = (*it);
					App->event_system->Send(ev);

					components_to_destroy.push_back(comp);

					ret = true;
					break;
				}
			}
		}
	}

	return ret;
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

std::string GameObject::GetUniqueId()
{
	return unique_id;
}

const bool GameObject::GetSelected() const
{
	return selected;
}

const bool GameObject::GetStatic() const
{
	return is_static;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

std::vector<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

const uint GameObject::GetChildsCount() const
{
	return childs.size();
}

void GameObject::SetParent(GameObject * new_parent)
{
	if (parent != nullptr)
	{
		for (vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
		{
			if ((*it) == this)
			{
				parent->childs.erase(it);
				break;
			}
		}
	}

	parent = nullptr;

	if (new_parent != nullptr)
	{
		if (!new_parent->HasChild(this))
		{
			new_parent->childs.push_back(this);
			parent = new_parent;
		}
	}

	if (parent == nullptr)
	{
		SetParent(App->gameobj->GetRoot());
	}
}

void GameObject::EraseParent()
{
	if (parent != nullptr)
	{
		for (vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
		{
			if ((*it) == this)
			{
				parent->childs.erase(it);
				break;
			}
		}

		parent = nullptr;

		SetParent(App->gameobj->GetRoot());
	}
}

bool GameObject::HasChild(GameObject * child)
{
	bool ret = false;

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it) == child)
		{
			ret = true;
			break;
		}
	}

	return ret;
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
 
	}
	
	if (!childs.empty())
	{
		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->RecursiveCalcBBox();
		}
	}
}

void GameObject::RecursiveTestRay(const LineSegment & segment, GameObject*& closest, float& dist)
{
	// Check if intersects with bbox
	if (local_bbox.IsFinite())
	{
		if (segment.Intersects(local_bbox))
		{
			// Get mesh
			ComponentMesh* cmesh = nullptr;
			cmesh = (ComponentMesh*)GetComponent(MESH);

			if (cmesh != nullptr)
			{
				if (cmesh->HasMesh())
				{
					// Transform segment to match mesh transform
					LineSegment segment_local_space(segment);
					segment_local_space.Transform(transform->GetGlobalTransform().Inverted());

					ResourceMesh* mesh = cmesh->GetMesh();

					// Check every triangle
					Triangle tri;
					uint* indices = mesh->GetIndices();
					float* vertices = mesh->GetVertices();
					for (int i = 0; i < mesh->GetNumIndices();)
					{
						tri.a.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;
						tri.b.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;
						tri.c.Set(vertices[(indices[i])], vertices[(indices[i] + 1)], vertices[(indices[i] + 2)]); ++i;

						float distance;
						float3 hit_point;

						if (segment_local_space.Intersects(tri, &distance, &hit_point))
						{
							if (distance < dist)
							{
								dist = distance;
								closest = this;
							}
						}
					}
				}
			}
		}
	}

	if (!childs.empty())
	{
		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			(*it)->RecursiveTestRay(segment, closest, dist);
		}
	}
}

void GameObject::TestRay(const LineSegment& segment, bool& hit, float& dist)
{
	dist = 9999999999999;
	hit = false;

	// Check if intersects with bbox
	if (local_bbox.IsFinite())
	{
		if (segment.Intersects(local_bbox))
		{
			// Get mesh
			ComponentMesh* cmesh = nullptr;
			cmesh = (ComponentMesh*)GetComponent(MESH);

			if (cmesh != nullptr)
			{
				if (cmesh->HasMesh())
				{
					// Transform segment to match mesh transform
					LineSegment segment_local_space(segment);
					segment_local_space.Transform(transform->GetGlobalTransform().Inverted());

					ResourceMesh* mesh = cmesh->GetMesh();

					// Check every triangle
					Triangle tri;
					uint* indices = mesh->GetIndices();
					float* vertices = mesh->GetVertices();
					for (int i = 0; i < mesh->GetNumIndices();)
					{
						tri.a.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;
						tri.b.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;
						tri.c.Set(vertices[(3 * indices[i])], vertices[(3 * indices[i]) + 1], vertices[(3 * indices[i]) + 2]); ++i;

						float distance;
						float3 hit_point;

						if (segment_local_space.Intersects(tri, &distance, &hit_point))
						{
							if (distance < dist)
							{
								dist = distance;
								hit = true;
							}
						}
					}
				}
			}
		}
	}
}

float4x4 GameObject::GetParentTransform()
{
	float4x4 trans = float4x4::identity;

	if (parent != nullptr)
		trans = parent->transform->GetGlobalTransform();
	
	return trans;
}

AABB GameObject::GetBbox() const
{
	return local_bbox;
}

void GameObject::OnLoadAbstraction(DataAbstraction & abs)
{
	name = abs.GetString("name");

	bool stat = abs.GetBool("static");

	if (stat)
		App->gameobj->AddGameObjectToStatic(this);
}

void GameObject::OnSaveAbstraction(DataAbstraction & abs)
{
	// Set the name
	abs.AddString("name", name.c_str());

	abs.AddBool("static", is_static);
}

void GameObject::SetDraw(bool set)
{
	draw = set;
}

void GameObject::SetDebugDraw(bool set)
{
	debug_draw = set;
}
 
void GameObject::DrawBBox()
{
	if (!debug_draw)
		return;

	if (local_bbox.IsFinite())
	{	
		if(selected)
			DebugDraw(local_bbox, Red, true, 4.0f);
		else
			DebugDraw(local_bbox, Green, true, 4.0f);
	}
}

void GameObject::DestroyComponents()
{
	for (std::list<Component*>::iterator des = components_to_destroy.begin(); des != components_to_destroy.end();)
	{
		for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			if ((*it) == (*des))
			{
				(*it)->CleanUp();
				delete (*it);
				components.erase(it);

				break;
			}
		}

		des = components_to_destroy.erase(des);
	}
}

void Function(Event ev)
{
}
