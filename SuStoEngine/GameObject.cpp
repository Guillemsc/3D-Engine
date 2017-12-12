#include "GameObject.h"
#include "Functions.h"
#include "App.h"
#include "imgui.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "ComponentCanvas.h"
#include "ModuleGameObject.h"
#include "Inspector.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "JSONLoader.h"
#include "ResourceManager.h"

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
	LOG_OUTPUT("Game Object Created");

	SetName("GameObject");

	AddComponent(TRANSFORM);
	transform = (ComponentTransform*)GetComponent(TRANSFORM);
}

void GameObject::UpdateComponents()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if((*it)->GetEnabled())
			(*it)->Update();
	}
}

void GameObject::Draw()
{
	if (!draw)
		return;

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

void GameObject::UpdateLogic()
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
	for (vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		(*it)->CleanUp();
		RELEASE(*it);
		it = components.erase(it);
	}
}

Component* GameObject::AddComponent(ComponentType type, string unique_id)
{
	Component* ret = nullptr;

	if (ContainsComponent(type))
		return ret;

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

			if (is_static)
				go_module->RecalculateKDTree();
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
		case UI_CANVAS:
		{
			ret = new ComponentCanvas(this, new_id);
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

	return ret;
}

void GameObject::RemoveComponent(ComponentType type)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->GetType() == type)
		{
			if (type == MESH)
			{
				if (is_static)
					go_module->RecalculateKDTree();
			}

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

AABB GameObject::GetBbox() const
{
	return local_bbox;
}

void GameObject::OnLoadSerialize(JSON_Doc config)
{
	name = config.GetString("name");
	
	bool stat = config.GetBool("static");
	if (stat)
		App->gameobj->AddGameObjectToStatic(this);

	int components_count = config.GetArrayCount("Components");

	for (int i = 0; i < components_count; i++)
	{
		JSON_Doc comp_node = config.GetJsonNode();
		comp_node.MoveToSectionFromArray("Components", i);

		ComponentType type = static_cast<ComponentType>((int)comp_node.GetNumber("type", 0));

		if (type != TRANSFORM)
		{
			Component* c = AddComponent(type);
			c->OnLoadSerialize(comp_node);
		}
		else
			transform->OnLoadSerialize(comp_node);
	}
}

void GameObject::SetDraw(bool set)
{
	draw = set;
}

void GameObject::OnSaveSerialize(JSON_Doc doc)
{
	// Set the name
	doc.SetString("name", name.c_str());

	doc.SetBool("static", is_static);

	doc.SetArray("Components");

	// Save components
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		// Add and move to a new section on the components array
		JSON_Doc comp_doc = doc.GetJsonNode();
		comp_doc.AddSectionToArray("Components");
		comp_doc.MoveToSectionFromArray("Components", doc.GetArrayCount("Components") - 1);

		comp_doc.SetNumber("type", (*it)->GetType());
		(*it)->OnSaveSerialize(comp_doc);
	}
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


