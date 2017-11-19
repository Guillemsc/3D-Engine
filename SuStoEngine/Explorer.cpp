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
	folder_icon = loader.LoadTexture("UI/FOLDER_icon.png");
}

void Explorer::Draw()
{
	igBeginDock("Explorer", &visible, ImGuiWindowFlags_MenuBar);

	vector<string> files = App->file_system->GetFilesInPath(App->file_system->GetAssetsPath().c_str());
	files = OrderFiles(files);

	int i = 0;
	for (vector<string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		string path = App->file_system->GetPathFromFilePath((*it).c_str());
		string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
		string extension = App->file_system->GetFileExtension(filename.c_str());
		extension = ToLowerCase(extension);
		string name = App->file_system->GetFilenameWithoutExtension(filename.c_str(), false);

		if (TextCmp(filename.c_str(), ".") || TextCmp(filename.c_str(), ".."))
			continue;

		if (TextCmp(extension.c_str(), "meta") || TextCmp(extension.c_str(), "tga") || TextCmp(extension.c_str(), "prefab"))
			continue;

		if (TextCmp(extension.c_str(), "fbx")) {
			ImGui::ImageButtonWithTextDOWN((ImTextureID*)fbx_icon, name.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
			
			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
				App->LoadFile((*it).c_str());

		}
		else if (TextCmp(extension.c_str(), "png")) {
			ImGui::ImageButtonWithTextDOWN((ImTextureID*)png_icon, name.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
		}
		else if (TextCmp(extension.c_str(), "")) {
			ImGui::ImageButtonWithTextDOWN((ImTextureID*)folder_icon, name.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
		}

		//ImGui::SameLine();
		//ImGui::Text(filename.c_str());
		
		if (i < MAX_FILES_HORIZONTAL)
			ImGui::SameLine();

		i++;
	}

	igEndDock();
}

vector<string> Explorer::OrderFiles(vector<string> files)
{
	int size = files.size();
	vector<string> tmp;
	
	int i = 0;
	while (i < 4)
	{
		for (vector<string>::iterator it = files.begin(); it != files.end();)
		{
			string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
			string extension = App->file_system->GetFileExtension(filename.c_str());
			extension = ToLowerCase(extension);

			if (i != 3 && (TextCmp(filename.c_str(), ".") || TextCmp(filename.c_str(), "..")))
			{
				it++;
				continue;
			}

			switch (i)
			{
			case 0: // Folders
				if (!TextCmp(extension.c_str(), ""))
				{
					it++;
					continue;
				}
				break;
			case 1: // Fbx
				if (!TextCmp(extension.c_str(),  "fbx"))
				{
					it++;
					continue;
				}
				break;
			case 2: // Png
				if (!TextCmp(extension.c_str(), "png"))
				{
					it++;
					continue;
				}
				break;
			case 3: // Others
				break;
			}
			
			tmp.push_back(*it);
			it = files.erase(it);
		
		}

		i++;
	}

	return tmp;
}
