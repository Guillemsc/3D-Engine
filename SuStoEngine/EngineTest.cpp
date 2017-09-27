#include "EngineTest.h"
#include "App.h"
#include "imgui.h"
#include "GeometryMath.h"
#include "Primitive.h"
#include "Functions.h"
#include "Console.h"
#include "glut\glut.h"

EngineTest::EngineTest(bool start_enabled) : EditorElement(start_enabled)
{
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
	if (ImGui::Checkbox("FILL RENDER", &fill_mode))					ImGui::SameLine();
	if (ImGui::Checkbox("WIREFRAME RENDER", &wireframe_mode))		ImGui::SameLine();
	if (ImGui::Checkbox("POINT RENDER", &point_mode))				ImGui::SameLine();
	//---------------------------------
	// Slider
	if (point_mode)
	{
		ImGui::SliderFloat("Point Size", &point_size_slider, 0, 10);
		glPointSize(point_size_slider);
	}

	if (gl_depth && !glIsEnabled(GL_DEPTH_TEST))
		glEnable(GL_DEPTH_TEST);
	else if (!gl_depth && glIsEnabled(GL_DEPTH_TEST))
		glDisable(GL_DEPTH_TEST);
	
	if (gl_cull_face && !glIsEnabled(GL_CULL_FACE))				// https://www.khronos.org/opengl/wiki/Face_Culling
	{
		glCullFace(GL_FRONT); 									// void glCullFace(GLenum mode​); mode​ can be set to GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK.
		glEnable(GL_CULL_FACE);									// The latter will cull all triangles. Culling both faces will only cull triangles (since only they have faces).
	}	
	else if (!gl_cull_face && glIsEnabled(GL_CULL_FACE))
		glDisable(GL_CULL_FACE);

	// Lighting and Color Material
	/* _________________________________________________________________GOOD SETTINGS_________________________________________________________________
			Set GL_LIGHT_0's position to something like 45 degrees to the 'vertical'. Coordinate (1,1,0) should work nicely in most cases.
			Set GL_LIGHT_0's Ambient color to 0,0,0,1
			Set GL_LIGHT_0's Diffuse color to 1,1,1,1
			Set GL_LIGHT_0's Specular color to 1,1,1,1
			Set the glLightModel global ambient to 0.2, 0.2, 0.2, 1 (this is the default).
			Don't set any other glLight or glLightModel options - just let them default.
			Enable GL_LIGHTING and GL_LIGHT_0.
			Enable GL_COLOR_MATERIAL and set glColorMaterial to GL_AMBIENT_AND_DIFFUSE.This means that glMaterial will control the polygon's specular and 
				emission colours and the ambient and diffuse will both be set using glColor.
			Set the glMaterial Specular colour to 1, 1, 1, 1
			Set the glMaterial Emission colour to 0, 0, 0, 1
			Set the glColor to whatever colour you want each polygon to basically appear to be.That sets the Ambient and Diffuse to the same value - which is what you generally want.
	*/
	if (gl_lighting && !glIsEnabled(GL_LIGHTING))				// https://www.khronos.org/opengl/wiki/How_lighting_works
		glEnable(GL_LIGHTING);
	else if (!gl_lighting && glIsEnabled(GL_LIGHTING))
		glDisable(GL_LIGHTING);

	if (gl_color_material && !glIsEnabled(GL_COLOR_MATERIAL))	// https://www.khronos.org/opengl/wiki/How_lighting_works
	{	
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);		// void glColorMaterial (GLenum face, GLenum mode); 
		glEnable(GL_COLOR_MATERIAL);
	}
	else if (!gl_color_material && glIsEnabled(GL_COLOR_MATERIAL))
		glDisable(GL_COLOR_MATERIAL);

	if (gl_texture_2d && !glIsEnabled(GL_TEXTURE_2D))			// https://www.khronos.org/opengl/wiki/Texture
		glEnable(GL_TEXTURE_2D);
	else if (!gl_texture_2d && glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);

	// Full 
	if (fill_mode && poly_mode != gl_fill)					// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonMode.xml
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			// void glPolygonMode(GLenum face, GLenum mode);
		poly_mode = gl_fill;								// face:	Specifies the polygons that mode applies to. Must be GL_FRONT for front-facing polygons, 	
		wireframe_mode = false;								//			GL_BACK for back-facing polygons, or GL_FRONT_AND_BACK for front- and back-facing polygons.	
		point_mode = false;									// mode:	Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL.
	} 														//			The initial value is GL_FILL for both front- and back-facing polygons.	
															 
	// Wireframe											
	if (wireframe_mode && poly_mode != gl_line)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		poly_mode = gl_line;
		fill_mode = false;
		point_mode = false;
	}

	// Point											
	if (point_mode && poly_mode != gl_point)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		poly_mode = gl_point;
		fill_mode = false;
		wireframe_mode = false;
	}

	if (!point_mode && !wireframe_mode && !fill_mode)
		fill_mode = true;
}
