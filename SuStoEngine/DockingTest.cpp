#include "DockingTest.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "FBO.h"

DockingTest::DockingTest(bool start_enabled)
{
}

DockingTest::~DockingTest()
{
}

void DockingTest::Start()
{
	dock = getDockContext();
}

void DockingTest::Draw()
{
	bool a = true;
	bool b = true;

	igBeginDock("Game", &a, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), ImVec2(dock->getDock("Game", &a).size.x, dock->getDock("Game", &a).size.y), ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();

	if (first_time) {

		first_time = false;
	}
}
