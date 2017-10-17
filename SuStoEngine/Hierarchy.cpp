#include "Hierarchy.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "Inspector.h"
#include "ModuleInput.h"

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
	igBeginDock("Hierarchy", &visible, ImGuiWindowFlags_MenuBar);
	
	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("+"))
		{
			ImGui::MenuItem("Create empty GameObject", NULL, &create_empty_game_object);
			ImGui::MenuItem("Create Primitive", NULL, &create_cube_object);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	PrintGoAndChilds(App->gameobj->GetRoot());

	//vector<GameObject*> game_objects = App->gameobj->GetRoot()->GetChilds();

	//PrintChilds(App->gameobj->GetRoot());

	//for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	//{
	//	ImGui::TreeNodeEx((*it)->GetName()
	//}

	//vector<GameObject*> game_objects = App->gameobj->GetListGameObjects();
	//for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	//{
	//	ImGui::PushID((*it)->GetId());

	//	if (ImGui::TreeNodeEx((*it)->GetName(), 0))
	//	{
	//		// If ctrl is pressed do multiselection
	//		if (App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL))
	//		{
	//			App->gameobj->AddGameObjectToSelected((*it));
	//		}

	//		// If shift is pressed do fill gap selection
	//		else if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT))
	//		{

	//		}

	//		// Monoselection
	//		else
	//		{
	//			App->gameobj->ClearSelection();
	//			App->gameobj->AddGameObjectToSelected((*it));
	//		}
	//		ImGui::TreePop();
	//	}
	//	ImGui::PopID();
	//}
	

	igEndDock(); 

	//if (create_empty_game_object)
	//{
	//	GameObject* go = App->gameobj->Create();

	//	App->gameobj->ClearSelection();
	//	App->gameobj->AddGameObjectToSelected(go);

	//	create_empty_game_object = false;
	//}

	//if (create_cube_object)
	//{
	//	GameObject* go = App->gameobj->Create();
	//	go->AddComponent(PRIMITIVE);

	//	App->gameobj->ClearSelection();
	//	App->gameobj->AddGameObjectToSelected(go);

	//	create_cube_object = false;
	//}

}

void Hierarchy::PrintGoAndChilds(GameObject * go)
{
	if (go == nullptr)
		return;

	if (go == App->gameobj->GetRoot())
	{
		vector<GameObject*> childs = go->GetChilds();

		for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		{
			PrintGoAndChilds(*it);
		}
	}
	else
	{
		ImGui::PushID(go->GetId());

		if (ImGui::TreeNodeEx(go->GetName(), 0))
		{
			vector<GameObject*> childs = go->GetChilds();

			for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
			{
				PrintGoAndChilds(*it);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}
