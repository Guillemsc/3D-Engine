#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "App.h"

#include "imgui/imgui.h"
#include "imgui_impl_sdl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleImGui::ModuleImGui(bool start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Awake()
{
	bool ret = true;

	LOG("Loading ImGui");
	SetName("imgui");

	ret = ImGui_ImplSdlGL2_Init(App->window->window);

	return ret;
}

bool ModuleImGui::Start()
{
	bool ret = true;


	return ret;
}

bool ModuleImGui::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleImGui::Update()
{
	bool ret = true;

	ImGui::BeginMainMenuBar();
	ImGui::Text("Hellow");

	return ret;
}

bool ModuleImGui::CleanUp()
{
	bool ret = true;

	ImGui_ImplSdlGL2_Shutdown();

	return ret;
}

void ModuleImGui::ImGuiEvent(SDL_Event * ev)
{
	ImGui_ImplSdlGL2_ProcessEvent(ev);
}

void ModuleImGui::NewFrame()
{
	ImGui::NewFrame();
}

void ModuleImGui::Draw()
{
	ImGui::Render();
}
