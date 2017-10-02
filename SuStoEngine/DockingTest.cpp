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

	if (dock->getDock("Dock1", &a).status == dock->Status_Float)
	{
		
	}


	igBeginWorkspace();
	igDockDebugWindow();

	igBeginDock("Dock1", &a, 0);
	ImGui::Text("fuck the police");

	igEndDock();


	igBeginDock("Dock2", &b, 0);

	ImGui::Text("is dis the rial doking?");
	ImGui::Image((void*)App->renderer3D->fbo_texture->GetTexture(), ImVec2(dock->getDock("Dock1", &a).size.x, dock->getDock("Dock1", &a).size.y), ImVec2(0, 1), ImVec2(1, 0));

	igEndDock();

	igEndWorkspace();
}
