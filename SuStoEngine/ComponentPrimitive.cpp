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
	SetPrimitive(ICUBE);
}

void ComponentPrimitive::Update()
{
	switch (type)
	{
	/*case VCUBE:
		App->renderer3D->DrawVertexBuffer(type, vCube_vertices_count);
		break;*/
	//case ICUBE:
	//	App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, 2, iCube_index_count, 1);
	//	break;
	//case SPHERE:
	//	App->renderer3D->DrawIndexBuffer(GL_QUADS, &Sphere_indices[0], Sphere_indices.size(), &Sphere_vertices[0]);
	//	break;
	//case CYLINDER:
	//	App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, &Cylinder_indices[0], Cylinder_indices.size(), &Cylinder_vertices[0]);
	//	break;
	//case ARROW:
	//	break;
	//case RAY:
	//	App->renderer3D->DrawIndexBuffer(GL_LINES, &Ray_indices[0], 2, &Ray_vertices[0]);
	//	break;
	//case PLANE:
	//	App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, plane_index, plane_index_count, plane_vertices);
	//	break;
	//case FRUSTUM:
	//	App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, frustum_index, frustum_index_count, frustum_vertices);
	//	break;
	//case CONE:
	//	App->renderer3D->DrawIndexBuffer(GL_TRIANGLES, &Cone_indices[0], Cone_indices.size(), &Cone_vertices[0]);
	//	break;
	}
}

void ComponentPrimitive::CleanUp()
{
}

void ComponentPrimitive::SetPrimitive(const PrimitiveType& type)
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
	case PLANE:
		Plane();
		break;
	case RAY:
		Ray();
		break;
	case CYLINDER:
		Cylinder();
		break;
	case FRUSTUM:
		Frustum();
		break;
	case CONE:
		Cone();
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

	//App->renderer3D->LoadVertexBuffer(type, vCube_vertices, vCube_vertices_count);
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

	App->renderer3D->LoadBuffer(iCube_vertices, iCube_vertices_count);

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

	App->renderer3D->LoadBuffer(iCube_index, iCube_index_count);
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

void ComponentPrimitive::Plane()
{
	type = PLANE;

	plane_vertices_count = 36;
	
	static float v[36] =
	{
		0.5f,	0.f,	0.5f, // 0
		0.5f,	0.f,	-0.5f, // 1
		-0.5f,	0.f,	-0.5f, // 2

		-0.5f,	0.f,	0.5f, // 3
		-1.5f,	0.f,	-0.5f,//4
		-1.5f,	0.f,	0.5f,//5
	};

	plane_vertices = v;

	plane_index_count = 12;
	
	static unsigned int v2[12]
	{
		0,1,2, 2,3,0, // i, i+1, i+1 | i, i+1, i-3
		3,2,4, 4,5,3  // 
	};

	plane_index = v2;
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

void ComponentPrimitive::Ray()
{
	type = RAY;

	float start_x = 0;
	float start_y = 10;
	float start_z = 0;

	float end_x = 0;
	float end_y = -10;
	float end_z = 0;

	Ray_vertices[0] = start_x;
	Ray_vertices[1] = start_y;
	Ray_vertices[2] = start_z;

	Ray_vertices[3] = end_x;
	Ray_vertices[4] = end_y;
	Ray_vertices[5] = end_z;

	Ray_indices[0] = 0;
	Ray_indices[1] = 1;
}

void ComponentPrimitive::Frustum()
{
	type = FRUSTUM;

	frustum_vertices_count = 36;

	static float v[36] =
	{
		-5.f,	5.f,	0.f,		//0
		5.f,	5.f,	0.f,		//1
		-5.f,	-5.f,	0.f,		//2
		5.f,	-5.f,	0.f,		//3

		-20.f,	20.f,	-20.f,		//4
		20.f,	20.f,	-20.f,		//5
		-20.f,	-20.f,	-20.f,		//6
		20.f,	-20.f,	-20.f,		//7
	};

	frustum_vertices = v;

	frustum_index_count = 12;

	static unsigned int v2[36]
	{
		0,2,1,	2,3,1,
		7,4,5,	7,6,4,
		4,0,5,	5,0,1,
		5,1,7,	7,1,3,
		2,6,3,	3,6,7,
		2,4,6,	2,0,4
	};

	frustum_index = v2;
	/*frustrum vertices_indices =
		{
		-5.f, 5.f, 0.f,     //v0
			5.f, 5.f, 0.f,     //v1
			-5.f, -5.f, 0.f,    //v2
			5.f, -5.f, 0.f,    //v3

			-20.f, 20.f, -20.f, //v4
			20.f, 20.f, -20.f, //v5
			-20.f, -20.f, -20.f,//v6
			20.f, -20.f, -20.f,//v7

					 //INDICES
			0, 2, 1, 2, 3, 1,
			7, 4, 5, 7, 6, 4,
			4, 0, 5, 5, 0, 1,
			5, 1, 7, 7, 1, 3,
			2, 6, 3, 3, 6, 7,
			2, 4, 6, 2, 0, 4
			};*/
}

void ComponentPrimitive::Cone()
{
	type = CONE;

	float radius = 0.5f;
	float height = 1.0f;
	float strips = 15;

	float R = 360 / strips;

	for (int i = 0; i < strips; i++)
	{
		float x = cos(R*i*DEGTORAD) * radius;
		float y = -height / 2;
		float z = sin(R*i*DEGTORAD) * radius;

		Cone_vertices.push_back(x);
		Cone_vertices.push_back(y);
		Cone_vertices.push_back(z);
	}

	float x = 0;
	float y = -height / 2;
	float z = 0;

	Cone_vertices.push_back(x);
	Cone_vertices.push_back(y);
	Cone_vertices.push_back(z);

	x = 0;
	y = +height / 2;
	z = 0;

	Cone_vertices.push_back(x);
	Cone_vertices.push_back(y);
	Cone_vertices.push_back(z);


	for (int i = strips - 1; i >= 0; --i)
	{
		// Bottom
		Cone_indices.push_back(i);
		Cone_indices.push_back(strips);

		if (i != 0)
			Cone_indices.push_back(i - 1);
		else
			Cone_indices.push_back(strips - 1);

		//Sides
		Cone_indices.push_back(i);
		Cone_indices.push_back(strips+1);

		if (i != 0)
			Cone_indices.push_back(i - 1);
		else
			Cone_indices.push_back(strips - 1);
	}

}

void ComponentPrimitive::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());


	if (ImGui::Combo("Primitives", &current, "VCube\0ICube\0Sphere\0Cylinder\0Arrow\0Ray\0Plane\0Frustum\0Cone\0"))
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
