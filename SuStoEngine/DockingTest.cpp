#include "DockingTest.h"
#include "imgui.h"
#include "imgui_docking.h"

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
	igBeginWorkspace();
	igDockDebugWindow();

	bool a = true;
	bool b = true;

	igBeginDock("Dock1", &a, 0);
	
	ImGui::Text("fuck the police");

	igEndDock();


	igBeginDock("Dock2", &b, 0);

	ImGui::Text("is dis the rial doking?");
	if (dock->getDock("Dock1", &a).status == dock->Status_Dragged)
		ImGui::Text("fuck the police");

	igEndDock();

	igEndWorkspace();
}
