#include "EditorUI.h"
#include "Console.h"
#include "ModuleWindow.h"
#include "App.h"
#include "Primitive.h"
#include "GeometryMath.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Functions.h"

//https://github.com/ocornut/imgui/issues/351

EditorUI::EditorUI(bool enabled) : Module(enabled)
{
}

EditorUI::~EditorUI()
{
}

bool EditorUI::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading ImGui");
	SetName("Editor");
	ret = ImGui_ImplSdlGL2_Init(App->window->window);

	// Styles
	//	-default
	//	-black_white
	//  -gray
	//  -black_orange
	//  -blue_yellow
	LoadStyle("blue_yellow");

	// Initial range set
	range_demo.x = 0;
	range_demo.y = 100;

	return ret;
}

bool EditorUI::PreUpdate()
{
	bool ret = true;

	// ImGui new frame
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	return ret;
}

bool EditorUI::Update()
{
	bool ret = true;

	int win_width, win_height;
	App->window->GetWindowSize(win_width, win_height);

	ImGuiStyle * style = &ImGui::GetStyle();

	// Main Menu -------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4")) { App->EndApp(); }
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Console", "�", &App->console->visible);

			ImGui::MenuItem("Configuration", "P", &show_app_configuration);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About SuSto Engine", NULL, &show_app_about);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug") && App->GetDebugMode())
		{
			ImGui::MenuItem("Engine Tests", NULL, &show_test_window);

			ImGui::MenuItem("Test window", NULL, &show_imgui_test_window);

			ImGui::EndMenu();
		}

		ImGui::Text("Awake: %f", App->profiler->GetAwakeTime()); ImGui::SameLine();
		ImGui::Text("Start: %f", App->profiler->GetStartTime()); ImGui::SameLine();
		ImGui::Text("Fps: %d", App->profiler->GetFPS());
	
		ImGui::EndMainMenuBar();
	}
	// -------------------------------------

	// Configuration
	if (show_app_configuration)
	{
		Configuration();
	}

	// About
	if (show_app_about)
	{
		About();
	}

	// Test window imgui
	if (show_imgui_test_window)
	{
		ImGui::ShowTestWindow();
	}

	// Test window of the engine
	if (show_test_window)
	{
		TestEngine();
	}
	
	return ret;
}

bool EditorUI::PostUpdate()
{
	bool ret = true;

	// ImGui Draw
	ImGui::Render();

	return ret;
}

bool EditorUI::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Destroying ImGui");
	ImGui_ImplSdlGL2_Shutdown();

	return ret;
}

void EditorUI::ImGuiInput(SDL_Event* ev)
{
	// ImGui Input
	ImGui_ImplSdlGL2_ProcessEvent(ev);
}

void EditorUI::Configuration()
{
	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPosCenter(ImGuiCond_::ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Profiler", &show_app_configuration, ImGuiWindowFlags_::ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::CollapsingHeader("App");

		for (list<Module*>::iterator it = App->modules.begin(); it != App->modules.end(); it++)
		{
			if (ImGui::CollapsingHeader((*it)->GetName()))
			{
				(*it)->OnConfiguration();
			}
		}

		ImGui::End();
	}
}

void EditorUI::About()
{
	ImGui::Begin("About SuSto Engine", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("SuSto Engine v0.1.3");
	ImGui::Separator();
	ImGui::Text("By Guillem Sunyer and Simon Stoyanov.");
	ImGui::Text("3D Engine made for the 3D engines subject during the 3rd year of Game Design and Game Development degree at CITM,");
	if (ImGui::Button("Github Repository")) 
	{
		App->GoToBrowser("https://github.com/Guillemsc/3D-Engine");
	}
	ImGui::SameLine();
	if (ImGui::Button("Download Latest Release")) 
	{
		App->GoToBrowser("https://github.com/Guillemsc/3D-Engine/releases");
	}
	ImGui::SameLine();
	if (ImGui::Button("Report Issue")) 
	{
		App->GoToBrowser("https://github.com/Guillemsc/3D-Engine/issues");
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Libraries")) 
	{
		ImGui::Columns(3, "Name");
		ImGui::Separator();
		ImGui::Text("Use"); ImGui::NextColumn();
		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("Version"); ImGui::NextColumn();
		ImGui::Separator();
		const char* use[3] = { "Math", "UI", "File System" };
		const char* name[3] = { "MathGeoLib", "ImGui", "Parson" };
		const char* version[3] = { "v1.5", "v1.52", "---" };
		static int selected = -1;
		for (int i = 0; i < 3; i++)
		{
			ImGui::Text(use[i]); ImGui::NextColumn();
			ImGui::Text(name[i]); ImGui::NextColumn();
			ImGui::Text(version[i]); ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("License"))
	{
		ImGui::Text("MIT License");
		ImGui::Text("Copyright(c) 2017 Guillem Sunyer Caldu and Simon Stoyanov Beltran");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this software and associated documentation files(the 'Software'), to deal");
		ImGui::Text("in the Software without restriction, including without limitation the rights");
		ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and / or sell");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
		ImGui::Text("furnished to do so, subject to the following conditions :");

		ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "The above copyright notice and this permission notice shall be included in all");
		ImGui::TextColored({ 1, 0.2f, 0.2f, 1 }, "copies or substantial portions of the Software.");

		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
		ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
		ImGui::Text("SOFTWARE.");
	}


	ImGui::End();
}

void EditorUI::TestEngine() 
{
	ImGui::SetNextWindowSize(ImVec2(600, 680), ImGuiCond_::ImGuiSetCond_FirstUseEver);

	if (!ImGui::Begin("Engine Tests", &show_test_window, ImGuiWindowFlags_NoResize))
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

	ImGui::End();

}

void EditorUI::GeometryMathTest()
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

void EditorUI::GenerateRandomNumbers(float2 range, int quantity)
{
	for (int i = 0; i < quantity; ++i) 
	{
		float number_rounded = roundf(GetRandomValue(range.x, range.y) * 100) / 100;
		char number[255];
		snprintf(number, 255, "%.2f", number_rounded);
		App->console->AddLog(number);
	}
}

void EditorUI::LoadStyle(char * name)
{
	ImGuiStyle * style = &ImGui::GetStyle();

	if (name == "default")
	{

	}
	else if (name == "black_white")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}
	else if (name == "gray")
	{
		style->ChildWindowRounding = 3.f;
		style->GrabRounding = 0.f;
		style->WindowRounding = 0.f;
		style->ScrollbarRounding = 3.f;
		style->FrameRounding = 3.f;
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style->Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
	}
	else if (name == "black_orange")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.90f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.80f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.44f, 0.30f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.27f, 0.27f, 0.27f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.88f, 0.22f, 0.06f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.44f, 0.32f, 0.80f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.86f, 0.20f, 0.04f, 0.80f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.89f, 0.24f, 0.08f, 0.80f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.20f, 0.00f, 0.80f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.92f, 0.31f, 0.16f, 0.80f);
	}

	else if ("blue_yellow")
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.16f, 0.18f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.80f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.40f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.66f, 0.66f, 0.66f, 0.45f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.29f, 0.36f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ComboBg] = ImVec4(0.13f, 0.52f, 0.94f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.49f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.74f, 0.45f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.11f, 0.43f, 0.77f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.88f, 0.53f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.90f, 0.55f, 0.02f, 1.00f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.89f, 0.54f, 0.01f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style->Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.80f);
		style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.14f, 0.50f, 0.88f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.90f, 0.54f, 0.00f, 1.00f);
	}
}

void EditorUI::OnConfiguration()
{
	char title[25];
	std::vector<float> framerate = App->profiler->GetFramesVector();
	sprintf_s(title, 25, "Framerate %.1f", framerate[framerate.size() - 1]);
	ImGui::PlotHistogram("Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
}
