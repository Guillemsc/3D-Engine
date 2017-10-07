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

	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// … draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	igEndDock();
}
