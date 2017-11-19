#include "Explorer.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "JSONLoader.h"
#include "Functions.h"

Explorer::Explorer(bool start_enabled)
{
}

Explorer::~Explorer()
{
}

void Explorer::Start()
{

}

void Explorer::Draw()
{
	igBeginDock("Explorer", &visible, ImGuiWindowFlags_MenuBar);

	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetAssetsPath().c_str());

	for (vector<string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		string path = App->file_system->GetPathFromFilePath((*it).c_str());
		string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
		string extension = App->file_system->GetFileExtension(filename.c_str());
		string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());

		
		if (TextCmp(extension.c_str(), "meta") || TextCmp(extension.c_str(), "prefab"))
			continue;

		ImGui::Text(filename.c_str());

		// Menu
		ImGui::PushID((*it).c_str());
		if (ImGui::BeginPopupContextItem("HerarchyPopup"))
		{
			if (ImGui::Button("Load"))
			{
				App->LoadFile((*it).c_str());
			}

			ImGui::EndPopup();
		}
		ImGui::PopID();
	}

	igEndDock();
}
