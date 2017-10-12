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
	if (has_mesh);
		mesh->Draw();
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

void ComponentMesh::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());

	if (!has_mesh)
	{
		ImGui::Text("No mesh loaded");
		return;
	}

	ImGui::Text("Id vertices: %d", mesh->id_vertices);
	ImGui::Text("Num vertices: %d", mesh->num_vertices);
	ImGui::Text("Id indices %d:", mesh->id_indices);
	ImGui::Text("Num indices: %d", mesh->num_indices);
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
