#include "Hierarchy.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "Inspector.h"

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
			ImGui::MenuItem("Create GameObject", NULL, &create_game_object);
			ImGui::MenuItem("Create Cube GameObject", NULL, &create_cube_object);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	list<GameObject*> game_objects = App->gameobj->GetListGameObjects();
	for (list<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it) 
	{
		(*it)->HierarchyView();
		if (App->gameobj->selected_go[(*it)->GetId()] && App->editorUI->GetInspector()->is_selected)
		{
			App->editorUI->GetInspector()->SetGameObjectInspected(*it);
			App->editorUI->GetInspector()->is_selected = false;
		}
		else if (!App->gameobj->selected_go[(*it)->GetId()] && App->editorUI->GetInspector()->is_selected)
		{
			App->editorUI->GetInspector()->SetGameObjectInspected(nullptr);
			App->editorUI->GetInspector()->is_selected = false;
		}
	}

	igEndDock(); 

	if (create_game_object || create_cube_object) 
	{
		GameObject* tmp = App->gameobj->Create();

		if (create_cube_object) 
		{
			LOG_OUTPUT("Cube Created");
			
			create_cube_object = false;
		}

		create_game_object = false;
	}

}
