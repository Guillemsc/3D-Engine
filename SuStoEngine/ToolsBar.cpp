#include "ToolsBar.h"
#include "imgui.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleGameObject.h"
#include "SceneManager.h"

ToolsBar::ToolsBar(bool start_enabled)
{
}

ToolsBar::~ToolsBar()
{
}

void ToolsBar::Start()
{
}

void ToolsBar::Draw(uint flags)
{
	flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::SetNextWindowPos(ImVec2(-5, 20));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWindowSize().x + 10, 27));
	bool open = true;
	ImGui::Begin("tool_Bar", &open, flags);

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

	if (App->scene_manager->GetState() == SceneState::EDIT)
	{
		ImGui::SetCursorPos(ImVec2(800, 3));
		if (ImGui::Button("Play"))
		{
			App->scene_manager->Play();
		}

		ImGui::SetCursorPos(ImVec2(850, 6));
		ImGui::Text("Current: EDITING");
	}
	else if(App->scene_manager->GetState() == SceneState::PLAY)
	{
		ImGui::SetCursorPos(ImVec2(800, 3));
		if (ImGui::Button("STOP"))
		{
			App->scene_manager->Edit();
		}

		ImGui::SetCursorPos(ImVec2(847, 3));
		if (ImGui::Button("Pause"))
		{
			App->scene_manager->Pause();
		}

		ImGui::SetCursorPos(ImVec2(900, 3));
		if (ImGui::Button("Step"))
		{
			App->scene_manager->Step();
		}

		ImGui::SetCursorPos(ImVec2(950, 6));
		ImGui::Text("Current: PLAYING");
	}

	ImGui::SetCursorPos(ImVec2(1250, 3));
	if (ImGui::Checkbox("Draw KDTree", &draw_kdtree))
	{
		App->gameobj->SetDebugKDTree(draw_kdtree);
	}

	ImGui::SetCursorPos(ImVec2(1400, 3));
	if (ImGui::Checkbox("Draw BBoxes", &draw_bboxes))
	{
		App->gameobj->SetDebugBBoxes(draw_bboxes);
	}

	ImGui::End();
}
