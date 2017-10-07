#include "Inspector.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"

Inspector::Inspector(bool start_enabled)
{
}

Inspector::~Inspector()
{
}

void Inspector::Start()
{
}

void Inspector::Draw()
{
	static bool inspector_bool = true;

	if (!visible)
		return;

	igBeginDock("Inspector", &inspector_bool, ImGuiWindowFlags_MenuBar);

	
	igEndDock();
}
