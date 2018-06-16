#include "Game.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"
#include "ImGuizmo.h"
#include "Functions.h"
#include "ModuleCamera3D.h"

Game::Game(bool start_enabled)
{
}

Game::~Game()
{
}

void Game::Start()
{
	
	position = { 0, 0 };
}

void Game::Draw(uint flags)
{
	flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::BeginDock("Game", false, &visible, false, flags);
	
	ImGuizmo::SetDrawlist();

	position = float2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
	size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

	// Keep in mind that ImGui has w and z inverted (x, y, z, w)
	if (saved_size.x != size.x || saved_size.y != size.y)
	{
		saved_size.x = size.x;
		saved_size.y = size.y;

		App->renderer3D->OnResize(size.x, size.y);

		ImGuizmo::SetRect(position.x, position.y, size.x, size.y);
	}

	ImGui::Image((void*)App->camera->GetEditorCamera()->GetTextId(), ImVec2(size.x, size.y), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::EndDock();
}

void Game::CleanUp()
{
}

const Rect Game::GetRect() const
{
	return math::Rect(position.x, position.y, size.x, size.y);
}
