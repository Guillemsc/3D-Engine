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
	ImGui::BeginDock("Inspector", false, &visible, false, ImGuiWindowFlags_MenuBar);

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

		// Static selector --------------------------------------
		bool stat = selected[0]->GetStatic();
		if (ImGui::Checkbox("static", &stat))
		{
			if (stat)
			{
				if(selected[0]->GetChildsCount() > 0)
					ImGui::OpenPopup("Static Enable");
				else
					App->gameobj->AddGameObjectToStatic(selected[0]);
			}
			else
			{
				if (selected[0]->GetChildsCount() > 0)
					ImGui::OpenPopup("Static Disable");
				else
					App->gameobj->RemoveGameObjectFromStatic(selected[0]);
			}
		}

		if (ImGui::BeginPopupModal("Static Enable", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Do you want to enable static for all the child objects as well?");
			ImGui::Separator();

			if (ImGui::Button("Yes, change children", ImVec2(160, 0))) 
			{
				App->gameobj->AddGameObjectToStaticRecursive(selected[0]);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No, this object only", ImVec2(160, 0)))
			{
				App->gameobj->AddGameObjectToStatic(selected[0]);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{ ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Static Disable", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Do you want to disable static for all the child objects as well?");
			ImGui::Separator();

			if (ImGui::Button("Yes, change children", ImVec2(160, 0)))
			{
				App->gameobj->RemoveGameObjectFromStaticRecursive(selected[0]);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No, this object only", ImVec2(160, 0)))
			{
				App->gameobj->RemoveGameObjectFromStatic(selected[0]);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(130, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		// ----------------------------------------

		ImGui::TextWrapped("Unique id: %s", selected[0]->GetUniqueId().c_str());
 
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

			ImGui::PushID((*it)->GetUniqueId().c_str());
			bool enabled = (*it)->GetActive();
			if (ImGui::Checkbox("", &enabled))
			{
				(*it)->SetActive(enabled);
			}
			ImGui::SameLine();
			if (ImGui::Button("Remove"))
			{
				(*selected.begin())->RemoveComponent((*it));
				return;
			}
			ImGui::PopID();
			ImGui::SameLine();
			if (ImGui::CollapsingHeader((*it)->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if((*it)->GetActive())
					(*it)->InspectorDraw(same_components);
			}

			ImGui::Separator();
		}
	}
	
	ImGui::EndDock();
}
