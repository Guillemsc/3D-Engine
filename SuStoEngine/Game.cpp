#include "Game.h"

#include "imgui_docking.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"
#include "ImGuizmo.h"

Game::Game(bool start_enabled)
{
}

Game::~Game()
{
}

void Game::Start()
{
	dock = getDockContext();
	position = { 0, 0 };
}

void Game::Draw()
{
	igBeginDock("Game", &visible, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	size = float2(ImGui::GetContentRegionAvail().y, ImGui::GetContentRegionAvail().x);
	position = float2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

	// Keep in mind that ImGui has w and z inverted (x, y, z, w)
	if (saved_size.x != size.x || saved_size.x != size.y)
	{
		App->renderer3D->OnResize(size.y, size.x);

		saved_size.x = size.x;
		saved_size.y = size.y;

		/*ImGuizmo::SetRect(position.x, position.y, size.x, size.y);*/

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	}

	ImGui::Image((void*)App->renderer3D->GetScreenTexture(), ImVec2(size.y, size.x), ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();
}

void Game::CleanUp()
{
}

const float4 Game::GetRect() const
{
	return float4(position.x, position.y, size.x, size.y);
}
