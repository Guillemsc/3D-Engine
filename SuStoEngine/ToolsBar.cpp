#include "ToolsBar.h"
#include "imgui.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleGameObject.h"

ToolsBar::ToolsBar(bool start_enabled)
{
}

ToolsBar::~ToolsBar()
{
}

void ToolsBar::Start()
{
}

void ToolsBar::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(-5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWindowSize().x + 10, 27));
	bool open = true;
	ImGui::Begin("tool_Bar", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::SetCursorPos(ImVec2(20, 3));
	if (ImGui::Button("Move"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
	}

	ImGui::SetCursorPos(ImVec2(64, 3));
	if (ImGui::Button("Rotate"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::ROTATE);
	}

	ImGui::SetCursorPos(ImVec2(120, 3));
	if (ImGui::Button("Scale"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::SCALE);
	}

	ImGui::End();
}
