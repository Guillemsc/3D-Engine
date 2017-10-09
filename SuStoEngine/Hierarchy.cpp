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
	static bool hierarchy_bool = true;

	if (!visible)
		return;

	igBeginDock("Hierarchy", &hierarchy_bool, ImGuiWindowFlags_MenuBar);
	
	// Menu
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("+"))
		{
			ImGui::MenuItem("Create empty GameObject", NULL, &create_empty_game_object);
			ImGui::MenuItem("Create Cube GameObject", NULL, &create_cube_object);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	vector<GameObject*> game_objects = App->gameobj->GetListGameObjects();
	for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		ImGui::PushID((*it)->GetId());

		if(ImGui::Selectable((*it)->GetName(), (*it)->GetSelected()))
		{
			// If ctrl is pressed do multiselection
			if (App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL))
			{
				App->gameobj->AddGameObjectToSelected((*it));
			}

			// If shift is pressed do fill gap selection
			else if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT))
			{

			}

			// Monoselection
			else
			{
				App->gameobj->ClearSelection();
				App->gameobj->AddGameObjectToSelected((*it));
			}
		}

		ImGui::PopID();
	}

	igEndDock(); 

	if (create_empty_game_object)
	{
		App->gameobj->Create();
		create_empty_game_object = false;
	}

	if (create_cube_object)
	{
		GameObject* go = App->gameobj->Create();
		go->AddComponent(PRIMITIVE);

		create_cube_object = false;
	}

}
