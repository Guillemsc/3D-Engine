#include "Game.h"

#include "imgui_docking.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"

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
	}

	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), ImVec2(size.y, size.x), ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();
}

float4 Game::GetRect()
{
	return float4(position.x, position.y, size.x, size.y);
}
