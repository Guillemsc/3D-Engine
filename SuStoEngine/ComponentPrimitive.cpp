#include "ComponentPrimitive.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "imgui.h"
#include <vector>

ComponentPrimitive::ComponentPrimitive(GameObject * owner) : Component(PRIMITIVE, owner)
{
	LOG_OUTPUT("Component Primitive Created");
	SetName("Primitive");
}

ComponentPrimitive::~ComponentPrimitive()
{
}

void ComponentPrimitive::Start()
{
	SetPrimitive(VCUBE);
}

void ComponentPrimitive::Update()
{
	switch (type)
	{
	case VCUBE:
		App->renderer3D->DrawVertexBuffer(type, vCube_vertices_count);
		break;
	case ICUBE:
		App->renderer3D->DrawIndexBuffer(type, iCube_index, iCube_index_count, iCube_vertices);
		break;
	}
}

void ComponentPrimitive::CleanUp()
{
}

void ComponentPrimitive::SetPrimitive(PrimitiveType type)
{
	switch (type)
	{
	case VCUBE:
		VertexCube();
		break;
	case ICUBE:
		IndexCube();
		break;
	}
}

void ComponentPrimitive::VertexCube()
{
	type = VCUBE;

	vCube_vertices_count = 108;
	static float v[108] = 
	{
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,

		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
	};

	vCube_vertices = v;

	App->renderer3D->LoadVertexBuffer(type, vCube_vertices, vCube_vertices_count);
}

void ComponentPrimitive::IndexCube()
{
	type = ICUBE;

	iCube_vertices_count = 24;
	static float v[24] =
	{
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f
	};

	iCube_vertices = v;

	iCube_index_count = 36;
	static unsigned int v2[36]
	{ 
		0,1,2, 2,3,0,   
		0,3,4, 4,5,0,
		0,5,6, 6,1,0,
		1,6,7, 7,2,1,
		7,4,3, 3,2,7,
		4,7,6, 6,5,4 
	};

	iCube_index = v2;
}
void ComponentPrimitive::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());
}

void ComponentPrimitive::OnEnable()
{
}

void ComponentPrimitive::OnDisable()
{
}
