#include "Explorer.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "JSONLoader.h"

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
		string name = App->file_system->GetFileNameFromFilePath((*it).c_str());
		ImGui::Text(name.c_str());
	}

	igEndDock();
}
