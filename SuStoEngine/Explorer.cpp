#include "Explorer.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "JSONLoader.h"
#include "Functions.h"
#include "ResourceTextureLoader.h"

Explorer::Explorer(bool start_enabled)
{
}

Explorer::~Explorer()
{
}

void Explorer::Start()
{
	ResourceTextureLoader loader;
	fbx_icon = loader.LoadTexture("UI/FBX_icon.png");
	png_icon = loader.LoadTexture("UI/PNG_icon.png");
}

void Explorer::Draw()
{
	igBeginDock("Explorer", &visible, ImGuiWindowFlags_MenuBar);

	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetAssetsPath().c_str());

	int i = 0;
	for (vector<string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		string path = App->file_system->GetPathFromFilePath((*it).c_str());
		string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
		string extension = App->file_system->GetFileExtension(filename.c_str());
		string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());

		
		if (TextCmp(extension.c_str(), "meta") || TextCmp(extension.c_str(), "tga") || TextCmp(extension.c_str(), "prefab") || TextCmp(extension.c_str(), ""))
			continue;

		if (TextCmp(extension.c_str(), "fbx")) {
			ImGui::ImageButton((ImTextureID*)fbx_icon, ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0));
			
			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
				App->LoadFile((*it).c_str());
		}
		else if (TextCmp(extension.c_str(), "png")) {
			ImGui::ImageButton((ImTextureID*)png_icon, ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0));
		}
		
		ImGui::SameLine(0, SPACING);
		ImGui::Text(filename.c_str());

		if (i < MAX_FILES_HORIZONTAL)
			ImGui::SameLine();

		i++;
	}

	igEndDock();
}
