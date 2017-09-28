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
}

void DockingTest::Draw()
{
	igBeginWorkspace();
	igDockDebugWindow();

	static bool a = true;
	igBeginDock("Dock1", &a, 0);
	
	ImGui::Text("fuck the police");

	igEndDock();

	bool b = true;
	igBeginDock("Dock2", &b, 0);

	ImGui::Text("is dis the rial doking?");

	igEndDock();

	igEndWorkspace();
}
