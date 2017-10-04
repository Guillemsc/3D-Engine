#include "Game.h"
#include "imgui.h"
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
}

void Game::Draw()
{
	bool a = true;

	igBeginDock("Game", &a, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (0 != size.x && 0 != size.y) 
	{
		App->renderer3D->OnResize(size.x, size.y);
	}

	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();
}
