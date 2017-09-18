#include "EditorUI.h"
#include "ModuleWindow.h"
#include "App.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

EditorUI::EditorUI(bool enabled) : Module(enabled)
{
}

EditorUI::~EditorUI()
{
}

bool EditorUI::Awake()
{
	bool ret = true;

	LOG("Loading ImGui");
	ret = ImGui_ImplSdlGL2_Init(App->window->window);

	return ret;
}

bool EditorUI::PreUpdate()
{
	bool ret = true;

	// ImGui new frame
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

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

	LOG("Destroying ImGui");
	ImGui_ImplSdlGL2_Shutdown();

	return ret;
}

void EditorUI::ImGuiInput(SDL_Event* ev)
{
	// ImGui Input
	ImGui_ImplSdlGL2_ProcessEvent(ev);
}
