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

	App->renderer3D->LoadVertexBuffer(1, vertices, 108);
}

void Game::Draw()
{
	bool a = true;

	igBeginDock("Game", &a, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
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

float4 Game::GetRect()
{
	return float4(position.x, position.y, size.x, size.y);
}
