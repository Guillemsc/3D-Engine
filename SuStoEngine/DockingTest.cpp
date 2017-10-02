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
	
	ImVec2 size = ImGui::GetContentRegionAvail();

	if (0 != size.x && 0 != size.y) {
		App->renderer3D->OnResize(size.x, size.y);
	}

	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), size, ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();

	if (first_time) {

		first_time = false;
	}
}
