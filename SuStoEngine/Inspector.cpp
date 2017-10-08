#include "Inspector.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "GameObject.h"
#include "App.h"
#include "ModuleGameObject.h"

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

	vector<GameObject*> selected = App->gameobj->GetSelectedGameObjects();

	if (selected.size() == 1)
	{
		// Draw
	}
	else if (selected.size() > 1)
	{
		for (vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
		{

		}

		// Draw taking in account multiple GameObjects selected !!!!
	}

	//if (selected_go)
	//{
	//	ImGui::BeginMenuBar();
	//		if (ImGui::BeginMenu("Add Component"))
	//		{
	//			if (ImGui::MenuItem("Add Transform"))
	//				selected_go->AddComponent(TRANSFORM);
	//			if (ImGui::MenuItem("Add Primitive"))
	//				selected_go->AddComponent(PRIMITIVE);
	//			ImGui::MenuItem("Add Other1");
	//			ImGui::MenuItem("Add Other2");
	//			ImGui::EndMenu();
	//		}
	//	ImGui::EndMenuBar();

	//	// Text Input To Rename the GameObject
	//	char name[25];
	//	sprintf_s(name, 25, selected_go->GetName());
	//	if (ImGui::InputText("", name, 25, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
	//		selected_go->SetName(name);

	//	ImGui::Separator();

	//	list<Component*> components = selected_go->GetComponents();
	//	for (list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	//	{
	//		(*it)->InspectorDraw();
	//	}
	//	
	//	
	//	
	//}
	
	igEndDock();
}
