#include "Hierarchy.h"
#include "imgui.h"
#include "imgui_docking.h"

Hierarchy::Hierarchy(bool start_enabled)
{
}

Hierarchy::~Hierarchy()
{
}

void Hierarchy::Start()
{
}

void Hierarchy::Draw()
{
	static bool hierarchy_bool = true;

	if (!visible)
		return;

	igBeginDock("Hierarchy", &hierarchy_bool, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_MenuBar);
	
	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("+"))
		{
			ImGui::MenuItem("Create GameObject");

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	igEndDock();
	
	

}
