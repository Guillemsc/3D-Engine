#include "Inspector.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "GameObject.h"
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

	if (selected_go)
	{
		ImGui::Text("GameObject_%d", selected_go->GetId());
	}
	
	igEndDock();
}

void Inspector::SetGameObjectInspected(GameObject * go)
{
	selected_go = go;
}
