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
	
	vector<GameObject*> game_objects = App->gameobj->GetRoot()->GetChilds();

	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
	{
		PrintGoAndChildsRecursive((*it));
	}

	igEndDock(); 

	if (create_empty_game_object)
	{
		GameObject* go = App->gameobj->Create();

		App->gameobj->ClearSelection();
		App->gameobj->AddGameObjectToSelected(go);

		create_empty_game_object = false;
	}

	if (create_cube_object)
	{
		GameObject* go = App->gameobj->Create();
		go->AddComponent(PRIMITIVE);

		App->gameobj->ClearSelection();
		App->gameobj->AddGameObjectToSelected(go);

		create_cube_object = false;
	}

}

void Hierarchy::PrintGoAndChildsRecursive(GameObject * go)
{
	if (go == nullptr)
		return;

	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if(go->GetSelected())
		flags |= ImGuiTreeNodeFlags_Selected;

	if (go->GetChildsCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool opened = ImGui::TreeNodeEx(go->GetName(), flags);

	// Input
	if (ImGui::IsItemClicked(0))
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
			App->gameobj->ClearSelection();
			App->gameobj->AddGameObjectToSelected(go);
		}
	}

	// Recursive draw childs
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
