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
	igBeginDock("Inspector", &visible, ImGuiWindowFlags_MenuBar);

	vector<GameObject*> selected = App->gameobj->GetSelectedGameObjects();

	ImGui::Separator();

	if (selected.size() >= 1)
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Add Component"))
		{
			if (ImGui::MenuItem("Add Material"))
				selected[0]->AddComponent(MATERIAL);
			if (ImGui::MenuItem("Add Mesh"))
				selected[0]->AddComponent(MESH);
			if (ImGui::MenuItem("Add Camera"))
				selected[0]->AddComponent(CAMERA);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();

		// Text rename
		char name[25];
		sprintf_s(name, 25, selected[0]->GetName());
		if (ImGui::InputText("", name, 25, ImGuiInputTextFlags_AutoSelectAll))
			selected[0]->SetName(name);

		if (ImGui::Checkbox("static", &selected[0]->static_))
		{
			if (selected[0]->static_) {
				LOG_OUTPUT("Gameobject set to static");
			}
		}

		ImGui::Separator();

		vector<Component*> components = selected[0]->GetComponents();

		for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			vector<Component*> same_components;

			if (selected.size() > 1)
			{
				for (vector<GameObject*>::iterator obj = ++selected.begin(); obj != selected.end(); obj++)
				{
					Component* comp = (*obj)->GetComponent((*it)->GetType());

					if (comp != nullptr)
					{
						same_components.push_back(comp);
					}
				}
			}

			ImGui::PushID((*it)->GetName());
			bool enabled = (*it)->GetEnabled();
			if (ImGui::Checkbox("", &enabled))
			{
				(*it)->SetEnabled(enabled);
			}
			ImGui::PopID();
			ImGui::SameLine();
			if (ImGui::CollapsingHeader((*it)->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if((*it)->GetEnabled())
					(*it)->InspectorDraw(same_components);
			}

			ImGui::Separator();
		}
	}
	
	igEndDock();
}
