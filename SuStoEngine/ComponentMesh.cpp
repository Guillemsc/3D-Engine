#include "ComponentMesh.h"
#include "App.h"
#include "GeometryLoader.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "GeometryMath.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "imgui.h"

ComponentMesh::ComponentMesh(GameObject * owner) : Component(MESH, owner)
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
	if(has_mesh)
		App->geometry->UnloadFile(mesh);
}

void ComponentMesh::SetMesh(Mesh* _mesh)
{
	mesh = _mesh;

	if (mesh != nullptr)
	{
		has_mesh = true;
	}
}

Mesh * ComponentMesh::GetMesh() const
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

	ImGui::Text("Id vertices: %d", mesh->GetIdVertices());
	ImGui::Text("Num vertices: %d", mesh->GetNumVertices());
	ImGui::Text("Id indices: %d", mesh->GetIdIndices());
	ImGui::Text("Num indices: %d", mesh->GetNumIndices());
	ImGui::Text("Id uv: %d", mesh->GetIdUV());
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
