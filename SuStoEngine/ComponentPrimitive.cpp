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
		App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, iCube_index, iCube_index_count, iCube_vertices);
		break;
	case SPHERE:
		App->renderer3D->DrawIndexBuffer(GL_QUADS, &Sphere_indices[0], Sphere_indices.size(), &Sphere_vertices[0]);
		break;
	case CYLINDER:
		App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, &Cylinder_indices[0], Cylinder_indices.size(), &Cylinder_vertices[0]);
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
	case CYLINDER:
		Cylinder();
		break;
	}
}

void ComponentPrimitive::VertexCube()
{
	type = VCUBE;

	float size = 1;

	vCube_vertices_count = 108;
	static float v[108] = 
	{
		-size / 2, -size / 2, size / 2,
		size / 2, -size / 2, size / 2,
		size / 2, size / 2, size / 2,

		-size / 2, -size / 2, size / 2,
		size / 2, size / 2, size / 2,
		-size / 2, size / 2, size / 2,

		size / 2, -size / 2, size / 2,
		size / 2, -size / 2, -size / 2,
		size / 2, size / 2, size / 2,

		size / 2, -size / 2, -size / 2,
		size / 2, size / 2, -size / 2,
		size / 2, size / 2, size / 2,

		size / 2, -size / 2, -size / 2,
		-size / 2, size / 2, -size / 2,
		size / 2, size / 2, -size / 2,

		-size / 2, size / 2, -size / 2,
		size / 2, -size / 2, -size / 2,
		-size / 2, -size / 2, -size / 2,

		-size / 2, -size / 2, -size / 2,
		-size / 2, size / 2, -size / 2,
		-size / 2, size / 2, size / 2,

		-size / 2, -size / 2, size / 2,
		-size / 2, -size / 2, -size / 2,
		-size / 2, size / 2, size / 2,

		size / 2, size / 2, size / 2,
		size / 2, size / 2, -size / 2,
		-size / 2, size / 2, size / 2,

		-size / 2, size / 2, -size / 2,
		-size / 2, size / 2, size / 2,
		size / 2, size / 2, -size / 2,

		-size / 2, -size / 2, size / 2,
		size / 2, -size / 2, size / 2,
		size / 2, -size / 2, -size / 2,

		-size / 2, -size / 2, -size / 2,
		-size / 2, -size / 2, size / 2,
		size / 2, -size / 2, -size / 2,
	};

	vCube_vertices = v;

	App->renderer3D->LoadVertexBuffer(type, vCube_vertices, vCube_vertices_count);
}

void ComponentPrimitive::IndexCube()
{
	type = ICUBE;

	float size = 1;

	iCube_vertices_count = 24;
	static float v[24] =
	{
		size / 2, size / 2, size / 2,
		-size / 2, size / 2, size / 2,
		-size / 2, -size / 2, size / 2,
		size / 2, -size / 2, size / 2,
		size / 2, -size / 2, -size / 2,
		size / 2, size / 2, -size / 2,
		-size / 2, size / 2, -size / 2,
		-size / 2, -size / 2, -size / 2
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

	Sphere_vertices.resize(rings * sectors * 3);

	std::vector<float>::iterator v = Sphere_vertices.begin();

	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) 
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;
	}

	Sphere_indices.resize(rings * sectors * 4);

	std::vector<uint>::iterator i = Sphere_indices.begin();

	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) 
	{
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
}
void ComponentPrimitive::Cylinder()
{
	type = CYLINDER;

	float radius = 0.5f;
	float height = 1.0f;
	float strips = 15;

	float R = 360 / strips;

	for (int i = 0; i < strips; i++)
	{
		float x = cos(R*i*DEGTORAD) * radius;
		float y = -height /2;
		float z = sin(R*i*DEGTORAD) * radius;

		Cylinder_vertices.push_back(x);
		Cylinder_vertices.push_back(y);
		Cylinder_vertices.push_back(z);
	}

	for (int i = 0; i < strips; i++)
	{
		float x = cos(R*i*DEGTORAD) * radius;
		float y = height / 2;
		float z = sin(R*i*DEGTORAD) * radius;

		Cylinder_vertices.push_back(x);
		Cylinder_vertices.push_back(y);
		Cylinder_vertices.push_back(z);
	}

	float x = 0;
	float y = -height / 2;
	float z = 0;

	Cylinder_vertices.push_back(x);
	Cylinder_vertices.push_back(y);
	Cylinder_vertices.push_back(z);

	x = 0;
	y = height / 2;
	z = 0;

	Cylinder_vertices.push_back(x);
	Cylinder_vertices.push_back(y);
	Cylinder_vertices.push_back(z);

	for (int i = strips - 1; i >= 0; --i)
	{
		// Body
		Cylinder_indices.push_back(i);
		Cylinder_indices.push_back(i + strips);

		if (i != 0)
		{
			Cylinder_indices.push_back(i + strips - 1);
		}
		else
		{
			Cylinder_indices.push_back(strips*2 - 1);
		}

		Cylinder_indices.push_back(i);

		if (i != 0)
		{
			Cylinder_indices.push_back(i + strips - 1);
			Cylinder_indices.push_back(i - 1);
		}
		else
		{
			Cylinder_indices.push_back(strips * 2 - 1);
			Cylinder_indices.push_back(strips - 1);
		}


		// Bottom
		Cylinder_indices.push_back(i);
		Cylinder_indices.push_back(strips - 1 + 1);

		if (i != 0)
			Cylinder_indices.push_back(i - 1);
		else
			Cylinder_indices.push_back(strips - 1);

		// Top
		Cylinder_indices.push_back(i + strips);
		Cylinder_indices.push_back(strips - 1 + 2);

		if (i != 0)
			Cylinder_indices.push_back(i + strips - 1);
		else
			Cylinder_indices.push_back(strips + strips - 1);
	}

	int i = 0;
}

void ComponentPrimitive::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());

	if (ImGui::Combo("Primitives", &current, "VCube\0ICube\0Sphere\0Cylinder\0Arrow\0Ray\0Plane"))
	{
		SetPrimitive(static_cast<PrimitiveType>(current + 1));
	}
}

void ComponentPrimitive::OnEnable()
{
}

void ComponentPrimitive::OnDisable()
{
}
