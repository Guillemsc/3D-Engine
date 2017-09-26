#include "EngineTest.h"
#include "App.h"
#include "imgui.h"
#include "GeometryMath.h"
#include "Primitive.h"
#include "Functions.h"
#include "Console.h"
#include "glut\glut.h"

EngineTest::EngineTest(bool start_enabled)
{
	visible = start_enabled;
}

EngineTest::~EngineTest()
{
}

void EngineTest::Start()
{
	// Initial range set
	range_demo.x = 0;
	range_demo.y = 100;
}

void EngineTest::Draw()
{
	if (!visible)
		return;

	ImGui::SetNextWindowSize(ImVec2(600, 680), 4);

	if (!ImGui::Begin("Engine Tests", &visible, ImGuiWindowFlags_NoResize))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Random Number Generation"))
	{
		ImGui::InputFloat2("Min | Max", range_demo.ptr(), 2);
		ImGui::InputInt("Number of generations", &quantity_demo);

		if (ImGui::Button("Generate", { 400, 30 }))
		{
			GenerateRandomNumbers(range_demo, quantity_demo);
		}

	}

	if (ImGui::CollapsingHeader("Geometry math test"))
	{
		GeometryMathTest();
	}

	if (ImGui::CollapsingHeader("OpenGL options"))
	{
		OpenGLOptions();
	}

	ImGui::End();
}

void EngineTest::GenerateRandomNumbers(float2 range, int quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		float number_rounded = roundf(GetRandomValue(range.x, range.y) * 100) / 100;
		char number[255];
		snprintf(number, 255, "%.2f", number_rounded);
		App->editorUI->GetConsole()->AddLog(number);
	}
}

void EngineTest::GeometryMathTest()
{
	ImGui::Text("Contact: %s", contact ? "Yes" : "No");

	ImGui::Separator();

	ImGui::InputFloat3("Position", vec3a);
	ImGui::SliderFloat("Distance", &dist, 0, 10);

	if (ImGui::Button("Sphere - Sphere"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_sphere_sphere = !contact_sphere_sphere;
	}

	if (ImGui::Button("Sphere - Capsule"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_sphere_capsules = !contact_sphere_capsules;
	}

	if (ImGui::Button("AABB - AABB"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_aabb_aabb = !contact_aabb_aabb;
	}

	if (ImGui::Button("OBB - OBB"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_obb_obb = !contact_obb_obb;
	}

	if (ImGui::Button("AABB - Ray"))
	{
		contact_sphere_sphere = false;
		contact_sphere_capsules = false;
		contact_aabb_aabb = false;
		contact_obb_obb = false;
		contact_aabb_ray = false;
		contact_aabb_ray = !contact_aabb_ray;
	}

	math::float3 p1 = { vec3a[0] - dist / 2, vec3a[1], vec3a[2] };
	math::float3 p2 = { vec3a[0] + dist / 2, vec3a[1], vec3a[2] };
	contact = false;

	if (contact_sphere_sphere)
	{
		ImGui::Text("contact_sphere_sphere");

		PSphere s1(2);
		s1.color.Set(255, 0, 0);
		s1.SetPos(p1.x, p1.y, p1.z);
		s1.Render();

		Sphere sph1(p1, 2);

		PSphere s2(2);
		s2.color.Set(0, 255, 0);
		s2.SetPos(p2.x, p2.y, p2.z);
		s2.Render();

		Sphere sph2(p2, 2);

		if (sph1.Intersects(sph2))
			contact = true;
	}

	if (contact_sphere_capsules)
	{
		ImGui::Text("contact_sphere_capsules");

		PSphere s1(2);
		s1.color.Set(255, 0, 0);
		s1.SetPos(p1.x, p1.y, p1.z);
		s1.Render();

		Sphere sph1(p1, 2);

		PCube s2(2, 2, 2);
		s2.color.Set(0, 255, 0);
		s2.SetPos(p2.x, p2.y, p2.z);
		s2.Render();

		Capsule c(float3(p2.x, p2.y - 1, p2.z), float3(p2.x, p2.y + 1, p2.z), 1.0f);

		if (sph1.Intersects(c))
			contact = true;
	}

	if (contact_aabb_aabb)
	{
		ImGui::Text("contact_aabb_aabb");

		PCube c1(2, 2, 2);
		c1.color.Set(255, 0, 0);
		c1.SetPos(p1.x, p1.y, p1.z);
		c1.Render();

		AABB sph1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));

		PCube c2(2, 2, 2);
		c2.color.Set(0, 255, 0);
		c2.SetPos(p2.x, p2.y, p2.z);
		c2.Render();

		AABB sph2(float3(p2.x - 1, p2.y - 1, p2.z - 1), float3(p2.x + 1, p2.y + 1, p2.z + 1));

		if (sph1.Intersects(sph2))
			contact = true;
	}

	if (contact_obb_obb)
	{
		ImGui::Text("contact_obb_obb");

		PCube c1(2, 2, 2);
		c1.color.Set(255, 0, 0);
		c1.SetPos(p1.x, p1.y, p1.z);
		c1.Render();

		AABB ab1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));
		OBB sph1(ab1);

		PCube c2(2, 2, 2);
		c2.color.Set(0, 255, 0);
		c2.SetPos(p2.x, p2.y, p2.z);
		c2.Render();

		AABB ab2(float3(p2.x - 1, p2.y - 1, p2.z - 1), float3(p2.x + 1, p2.y + 1, p2.z + 1));
		OBB sph2(ab2);

		if (sph1.Intersects(sph2))
			contact = true;
	}

	if (contact_aabb_ray)
	{
		ImGui::Text("contact_aabb_ray");

		PCube c1(2, 2, 2);
		c1.color.Set(255, 0, 0);
		c1.SetPos(p1.x, p1.y, p1.z);
		c1.Render();

		AABB sph1(float3(p1.x - 1, p1.y - 1, p1.z - 1), float3(p1.x + 1, p1.y + 1, p1.z + 1));

		PLine l1(p2.x, p2.y, p2.z + 10);
		l1.color.Set(255, 0, 0);
		l1.SetPos(p2.x, p2.y, p2.z - 10);
		l1.Render();

		Ray sph2(float3(p2.x, p2.y, p2.z - 10), float3(p2.x, p2.y, p2.z + 10));

		if (sph1.Intersects(sph2))
			contact = true;
	}
}

void EngineTest::OpenGLOptions()
{
	// GL_DEPTH_TEST		GL_CULL_FACE		GL_LIGHTING
	// GL_COLOR_MATERIAL	GL_TEXTURE_2D		two other
	// Wireframe

	// Checkboxes
	if (ImGui::Checkbox("GL_DEPTH_TEST", &gl_depth))				ImGui::SameLine();
	if (ImGui::Checkbox("GL_CULL_FACE", &gl_cull_face))				ImGui::SameLine();
	if (ImGui::Checkbox("GL_LIGHTING", &gl_lighting))				ImGui::SameLine();
	if (ImGui::Checkbox("GL_COLOR_MATERIAL", &gl_color_material))	ImGui::SameLine();
	if (ImGui::Checkbox("GL_TEXTURE_2D", &gl_texture_2d))			ImGui::SameLine();
	if (ImGui::Checkbox("WIREFRAME", &wireframe_mode))				ImGui::SameLine();
	//---------------------------------

	
	

}
