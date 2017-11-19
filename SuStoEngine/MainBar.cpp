#include "MainBar.h"
#include "imgui.h"
#include "App.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "EditorUI.h"
#include "About.h"
#include "Configuration.h"
#include "Console.h"
#include "EngineTest.h"
#include "Game.h"
#include "Hardware.h"
#include "Hierarchy.h"
#include "Inspector.h"
#include "ProfilerViewer.h"
#include "Functions.h"
#include "Explorer.h"

MainBar::MainBar(bool start_enabled) : EditorElement(start_enabled)
{
}

MainBar::~MainBar()
{
}

void MainBar::Start()
{
}

void MainBar::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open in Explorer", ""))
				App->GoToFolder(App->GetBasePath());

			ImGui::Separator();

			if (ImGui::MenuItem("Save Scene", ""))
				App->scene_manager->SaveScene("test.json");

			if (ImGui::MenuItem("Quit", "Alt+F4"))
				App->EndApp();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				App->gameobj->Create();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Component"))
		{
			if (App->gameobj->GetSelectedGameObjects().size() > 0)
			{
				vector<GameObject*> selected = App->gameobj->GetSelectedGameObjects();

				if (ImGui::MenuItem("Add Material"))
					selected[0]->AddComponent(MATERIAL);
				if (ImGui::MenuItem("Add Mesh"))
					selected[0]->AddComponent(MESH);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("Game", NULL, &App->editorUI->GetGame()->visible);

			ImGui::MenuItem("Hierarchy", NULL, &App->editorUI->GetHerarchy()->visible);

			ImGui::MenuItem("Inspector", NULL, &App->editorUI->GetInspector()->visible);

			ImGui::MenuItem("Explorer", NULL, &App->editorUI->GetExplorer()->visible);

			ImGui::MenuItem("Console", NULL, &App->editorUI->GetConsole()->visible);

			ImGui::MenuItem("Configuration", NULL, &App->editorUI->GetConfiguration()->visible);

			ImGui::MenuItem("Profiler", NULL, &App->editorUI->GetProfilerViewer()->visible);

			ImGui::MenuItem("Hardware", NULL, &App->editorUI->GetHardware()->visible);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About SuSto Engine", NULL, &App->editorUI->GetAbout()->visible);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Layouts"))
		{
			ImGui::Text("Current: %s", App->editorUI->GetCurrentLayout().c_str());

			ImGui::Separator();

			std::list<string> layouts = App->editorUI->GetLayouts();

			for (list<string>::iterator it = layouts.begin(); it != layouts.end();)
			{
				ImGui::Text((*it).c_str());

				ImGui::SameLine();

				string id = (*it) + "set";
				ImGui::PushID(id.c_str());
				if (ImGui::SmallButton("Set"))
				{
					App->editorUI->SaveCurrentLayout();
					App->editorUI->SetCurrentLayout((*it).c_str());
					App->editorUI->LoadCurrentLayout();
				}
				ImGui::PopID();

				ImGui::SameLine();

				id = (*it) + "x";
				ImGui::PushID(id.c_str());
				if (ImGui::SmallButton("x"))
				{
					App->editorUI->SetCurrentLayout();
					App->editorUI->RemoveLayout((*it).c_str());
					it = layouts.erase(it);
					App->editorUI->SaveLayoutsInfo();
				}
				else
				{
					++it;
				}

				ImGui::PopID();
			}
			ImGui::Separator();

			ImGui::InputText("", new_layout_name, 254);
			if (ImGui::Button("Save layout"))
			{
				if (!TextCmp(new_layout_name, ""))
				{
					App->editorUI->SaveNewLayout(new_layout_name);
					App->editorUI->SaveLayoutsInfo();
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug") && App->GetDebugMode())
		{
			//ImGui::MenuItem("Engine Tests", NULL, &engine_test->visible);

			ImGui::MenuItem("Test window", NULL, &show_imgui_test_window);

			ImGui::EndMenu();
		}

		ImGui::Text("Fps: %d", App->profiler->GetFPS());

		ImGui::EndMainMenuBar();
	}
	// -------------------------------------

	// Test window imgui
	if (show_imgui_test_window)
	{
		ImGui::ShowTestWindow();
	}
}
