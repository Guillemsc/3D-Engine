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

	static float vertices[108] = {
		10.f, 0.f, 0.f,
		10.f, 0.f, 5.f,
		10.f, 5.f, 0.f,
		10.f, 0.f, 5.f,
		10.f, 5.f, 5.f,
		10.f, 5.f, 0.f,
		10.f, 0.f, 5.f,
		15.f, 0.f, 5.f,
		10.f, 5.f, 5.f,
		15.f, 0.f, 5.f,
		15.f, 5.f, 5.f,
		10.f, 5.f, 5.f,
		15.f, 0.f, 5.f,
		15.f, 0.f, 0.f,
		15.f, 5.f, 5.f,
		15.f, 0.f, 0.f,
		15.f, 5.f, 0.f,
		15.f, 5.f, 5.f,
		15.f, 0.f, 0.f,
		10.f, 0.f, 0.f,
		10.f, 5.f, 0.f,
		15.f, 0.f, 0.f,
		10.f, 5.f, 0.f,
		15.f, 5.f, 0.f,
		10.f, 5.f, 0.f,
		10.f, 5.f, 5.f,
		15.f, 5.f, 0.f,
		10.f, 5.f, 5.f,
		15.f, 5.f, 5.f,
		15.f, 5.f, 0.f,
		15.f, 0.f, 5.f,
		15.f, 0.f, 0.f,
		10.f, 0.f, 5.f,
		15.f, 0.f, 0.f,
		10.f, 0.f, 0.f,
		10.f, 0.f, 5.f
	};

	App->renderer3D->LoadBuffer(1, vertices, 108);
}

void Game::Draw()
{
	bool a = true;

	igBeginDock("Game", &a, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (win_size_x != size.x || win_size_y != size.y)
	{
		App->renderer3D->OnResize(size.x, size.y);

		win_size_x = size.x;
		win_size_y = size.y;
	}

	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));

	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	
	glColor3f(255.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 5.f);
	glVertex3f(0.f, 5.f, 0.f);
	
	glColor3f(0.f, 255.f, 0.f);
	glVertex3f(0.f, 0.f, 5.f);
	glVertex3f(0.f, 5.f, 5.f);
	glVertex3f(0.f, 5.f, 0.f); 

	glColor3f(0.f, 0.f, 255.f);
	glVertex3f(0.f, 0.f, 5.f);
	glVertex3f(5.f, 0.f, 5.f);
	glVertex3f(0.f, 5.f, 5.f);

	glColor3f(255.f, 255.f, 0.f);
	glVertex3f(5.f, 0.f, 5.f);
	glVertex3f(5.f, 5.f, 5.f);
	glVertex3f(0.f, 5.f, 5.f);

	glColor3f(0.f, 255.f, 255.f);
	glVertex3f(5.f, 0.f, 5.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(5.f, 5.f, 5.f);

	glColor3f(255.f, 0.f, 255.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);
	glVertex3f(5.f, 5.f, 5.f);

	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 5.f, 0.f);

	glColor3f(255.f, 255.f, 255.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 5.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);

	glColor3f(255.f, 255.f/2, 0.f);
	glVertex3f(0.f, 5.f, 0.f);
	glVertex3f(0.f, 5.f, 5.f);
	glVertex3f(5.f, 5.f, 0.f);

	glColor3f(0.f, 255.f/2, 255.f);
	glVertex3f(0.f, 5.f, 5.f);
	glVertex3f(5.f, 5.f, 5.f);
	glVertex3f(5.f, 5.f, 0.f);

	glColor3f(0.f, 255.f/2, 0.f);
	glVertex3f(5.f, 0.f, 5.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 5.f);

	glColor3f(0.f, 0.f, 255.f/2);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 5.f);
	glEnd();

	glLineWidth(1.0f);

	App->renderer3D->DrawBuffer(1, 108);

	igEndDock();
}
