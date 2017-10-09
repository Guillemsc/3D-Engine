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

	ImGui::Separator();

	if (selected.size() >= 1)
	{
		// Text rename
		char name[25];
		sprintf_s(name, 25, selected[0]->GetName());
		if (ImGui::InputText("", name, 25, ImGuiInputTextFlags_AutoSelectAll))
			selected[0]->SetName(name);

		ImGui::Separator();

		vector<Component*> components = selected[0]->GetComponents();

		for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			vector<Component*> same_components;

			if (selected.size() > 1)
			{
				for (vector<GameObject*>::iterator obj = ++selected.begin(); obj != selected.end(); obj++)
				{
					Component* comp = (*obj)->FindComponentByType((*it)->GetType());

					if (comp != nullptr)
					{
						same_components.push_back(comp);
					}
				}
			}

			(*it)->InspectorDraw(same_components);

			ImGui::Separator();
		}
	}
	
	igEndDock();
}
