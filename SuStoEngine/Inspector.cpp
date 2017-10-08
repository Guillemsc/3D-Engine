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
		// Text Input To Rename the GameObject
		char name[25];
		sprintf_s(name, 25, selected_go->GetName());
		if (ImGui::InputText("", name, 25, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			selected_go->SetName(name);

		
		ImGui::BeginMenuBar();


		ImGui::EndMenuBar();
	}
	
	igEndDock();
}

void Inspector::SetGameObjectInspected(GameObject * go)
{
	selected_go = go;
}
