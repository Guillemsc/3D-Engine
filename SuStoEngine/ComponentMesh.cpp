#include "ComponentMesh.h"
#include "App.h"
#include "GeometryLoader.h"
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
	has_mesh = true;
}

Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

void ComponentMesh::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());

	if (!has_mesh)
	{
		ImGui::Text("No mesh loaded");
		return;
	}

	ImGui::Text("Id vertices: %d", mesh->GetIdVertices());
	ImGui::Text("Num vertices: %d", mesh->GetNumVertices());
	ImGui::Text("Id indices: %d", mesh->GetIdIndices());
	ImGui::Text("Num indices: %d", mesh->GetNumIndices());
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
