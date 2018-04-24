#include "Hierarchy.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "Inspector.h"
#include "ModuleInput.h"
#include "JSONLoader.h"
#include "ResourceManager.h"
#include "ResourcePrefabLoader.h"

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
	ImGui::BeginDock("Hierarchy", false, &visible, false, ImGuiWindowFlags_MenuBar);
	
	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("+"))
		{
			ImGui::MenuItem("Create empty GameObject", NULL, &create_empty_game_object);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	// Draw game objects from root recursively
	vector<GameObject*> game_objects = App->gameobj->GetRoot()->GetChilds();

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		PrintGoAndChildsRecursive((*it));
	}

	ImGui::EndDock(); 

	// Create
	if (create_empty_game_object)
	{
		GameObject* go = App->gameobj->Create();

		App->gameobj->ClearSelection();
		App->gameobj->AddGameObjectToSelected(go);

		create_empty_game_object = false;
	}
}

void Hierarchy::PrintGoAndChildsRecursive(GameObject * go)
{
	if (go == nullptr)
		return;

	// ------------------------
	// Flags ------------------
	// ------------------------
	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if(go->GetSelected())
		flags |= ImGuiTreeNodeFlags_Selected;

	if (go->GetChildsCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	// ------------------------
	// Draw line --------------
	// ------------------------
	ImGui::PushID(go->GetUniqueId().c_str());
	bool opened = ImGui::TreeNodeEx(go->GetName(), flags);
	ImGui::PopID();

	// ------------------------
	// Input ------------------
	// ------------------------
	if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
	{
		//If ctrl is pressed do multiselection
		if (App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL))
		{
			App->gameobj->AddGameObjectToSelected(go);
		}

		// If shift is pressed do fill gap selection
		else if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT))
		{
		
		}

		// Monoselection
		else
		{
			if (!ImGui::IsItemClicked(1) || !go->GetSelected())
			{
				App->gameobj->ClearSelection();
				App->gameobj->AddGameObjectToSelected(go);
			}
		}
	}

	// Menu
	ImGui::PushID(go->GetUniqueId().c_str());
	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	{
		if (ImGui::Button("Delete"))
		{
			App->gameobj->DestroySelectedGameObjects();
		}

		if (ImGui::Button("Create Prefab"))
		{
			ResourcePrefabLoader* loader = (ResourcePrefabLoader*)App->resource_manager->GetLoader(ResourceType::RT_PREFAB);
			loader->CreatePrefab(*App->gameobj->GetSelectedGameObjects().begin());
		}

		ImGui::EndPopup();
	}
	ImGui::PopID();

	// ------------------------
	// Draw childs recursive --
	// ------------------------ 
	if (opened)
	{
		vector<GameObject*> childs = go->GetChilds();

		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			PrintGoAndChildsRecursive(*it);
		}

		ImGui::TreePop();
	}
}
