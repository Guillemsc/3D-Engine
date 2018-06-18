#include "ComponentMesh.h"
#include "App.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "GeometryMath.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ResourceMesh.h"
#include "ResourceManager.h"
#include "JSONLoader.h"
#include "imgui.h"

ComponentMesh::ComponentMesh(GameObject * owner, std::string unique_id) : Component(MESH, owner, unique_id)
{
	CONSOLE_LOG("Component Mesh Created");
	SetName("Mesh");

	SetOnePerGo(true);
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Start()
{
}

void ComponentMesh::Update()
{

}

void ComponentMesh::CleanUp()
{
	RemoveMesh();
}

void ComponentMesh::ChangeMesh(ResourceMesh * _mesh)
{
	if (mesh != _mesh)
	{
		RemoveMesh();
		SetMesh(_mesh);
	}
}

void ComponentMesh::SetMesh(ResourceMesh* _mesh)
{
	RemoveMesh();

	if (_mesh != nullptr)
	{
		mesh = _mesh;

		mesh->StartUsing();

		has_mesh = true;
	}
}

void ComponentMesh::RemoveMesh()
{
	if (mesh != nullptr)
	{
		mesh->StopUsing();

		mesh = nullptr;
		has_mesh = false;
	}
}

ResourceMesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

const bool ComponentMesh::HasMesh() const
{
	return has_mesh;
}

void ComponentMesh::OnGetBoundingBox(AABB& box)
{
	if (has_mesh)
	{
		box.Enclose(mesh->GetBBox());
	}
}

void ComponentMesh::InspectorDraw(std::vector<Component*> components)
{
	Resource* curr_res = mesh;
	if (App->resource_manager->DrawResourceSelector(GetUniqueId().c_str(), "Mesh", ResourceType::RT_MESH, curr_res))
	{
		ChangeMesh((ResourceMesh*)curr_res);
	}

	if (mesh != nullptr)
	{
		ImGui::TextWrapped("Unique id: %s", mesh->GetUniqueId().c_str());
		ImGui::Text("Used by: %d GameObjects", mesh->UsedCount());
		ImGui::Text("Id vertices: %d", mesh->GetIdVertices());
		ImGui::Text("Num vertices: %d", mesh->GetNumVertices());
		ImGui::Text("Id indices: %d", mesh->GetIdIndices());
		ImGui::Text("Num indices: %d", mesh->GetNumIndices());
	}
}

void ComponentMesh::OnLoadAbstraction(DataAbstraction& abs)
{
	string mesh_id = abs.GetString("mesh_id", "no_id");

	ResourceMesh* rmesh = (ResourceMesh*)App->resource_manager->Get(mesh_id);
	SetMesh(rmesh);
}

void ComponentMesh::OnSaveAbstraction(DataAbstraction& abs)
{	
	if(mesh != nullptr)
		abs.AddString("mesh_id", mesh->GetUniqueId().c_str());	
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
