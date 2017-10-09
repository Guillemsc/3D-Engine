#include "ComponentPrimitive.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include <gl/GL.h>
#include <gl/GLU.h>
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
	SetPrimitive(SPHERE);
}

void ComponentPrimitive::Update()
{
	switch (type)
	{
	case VCUBE:
		App->renderer3D->DrawVertexBuffer(type, vCube_vertices_count);
		break;
	case ICUBE:
		App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, iCube_index, iCube_index_count, iCube_vertices);
		break;
	case SPHERE:
		App->renderer3D->DrawIndexBuffer(GL_QUADS, &indices[0], indices.size(), &vertices[0]);
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
	case SPHERE:
		Sphere();
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
void ComponentPrimitive::Sphere()
{
	type = SPHERE;

	float radius = 0.5f;
	uint rings = 10;
	uint sectors = 10;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);

	std::vector<float>::iterator v = vertices.begin();

	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	indices.resize(rings * sectors * 4);

	std::vector<uint>::iterator i = indices.begin();

	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) 
	{
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
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
