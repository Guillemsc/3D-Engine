#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject * owner) : Component(MESH, owner)
{
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
		mesh.Draw();
}

void ComponentMesh::CleanUp()
{
}

void ComponentMesh::SetMesh(Mesh _mesh)
{
	mesh = _mesh;
	has_mesh = true;
}

void ComponentMesh::InspectorDraw(std::vector<Component*> components)
{
}

void ComponentMesh::OnEnable()
{
}

void ComponentMesh::OnDisable()
{
}
