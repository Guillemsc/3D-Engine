#include "ComponentMesh.h"
#include "App.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "GeometryMath.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ResourceMesh.h"
#include "JSONLoader.h"
#include "imgui.h"

ComponentMesh::ComponentMesh(GameObject * owner, std::string unique_id) : Component(MESH, owner, unique_id)
{
	LOG_OUTPUT("Component Mesh Created");
	SetName("Mesh");
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

void ComponentMesh::SetMesh(ResourceMesh* _mesh)
{
	RemoveMesh();

	mesh = _mesh;

	if (mesh != nullptr)
	{
		mesh->LoadMem();

		has_mesh = true;
	}
}

void ComponentMesh::RemoveMesh()
{
	if (mesh != nullptr)
	{
		mesh->UnloadMem();

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
	if (!has_mesh)
	{
		ImGui::Text("No mesh loaded");
		return;
	}

	ImGui::TextWrapped("Unique id: %s", mesh->GetUniqueId().c_str());
	ImGui::Text("Used by: %d GameObjects", mesh->UsedCount());
	ImGui::Text("Id vertices: %d", mesh->GetIdVertices());
	ImGui::Text("Num vertices: %d", mesh->GetNumVertices());
	ImGui::Text("Id indices: %d", mesh->GetIdIndices());
	ImGui::Text("Num indices: %d", mesh->GetNumIndices());
	ImGui::Text("Id uv: %d", mesh->GetIdUV());
}

void ComponentMesh::OnLoadScene(JSON_Doc * config)
{
}

void ComponentMesh::OnSaveScene(JSON_Doc * config)
{
	if (has_mesh)
	{
		config->SetString("mesh_id", mesh->GetUniqueId().c_str());
	}
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
