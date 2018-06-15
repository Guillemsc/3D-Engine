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

	// Transformation buttons -----------------------------------------

	bool to_change = false;

	if (App->gameobj->GetGuizmoOperation() == ImGuizmo::OPERATION::TRANSLATE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
		to_change = true;
	}

	ImGui::SetCursorPos(ImVec2(40, 6));
	if (ImGui::Button("TRANSLATE"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
	}

	if (to_change)
	{
		ImGui::PopStyleColor(1);
		to_change = false;
	}

	if (App->gameobj->GetGuizmoOperation() == ImGuizmo::OPERATION::ROTATE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
		to_change = true;
	}

	ImGui::SetCursorPos(ImVec2(114, 6));
	if (ImGui::Button("ROTATE"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::ROTATE);
	}

	if (to_change)
	{
		ImGui::PopStyleColor(1);
		to_change = false;
	}

	if (App->gameobj->GetGuizmoOperation() == ImGuizmo::OPERATION::SCALE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
		to_change = true;
	}

	ImGui::SetCursorPos(ImVec2(167, 6));
	if (ImGui::Button("SCALE"))
	{
		App->gameobj->SetGuizmoOperation(ImGuizmo::OPERATION::SCALE);
	}

	if (to_change)
	{
		ImGui::PopStyleColor(1);
		to_change = false;
	}

	// ----------------------------------------------------------------

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
