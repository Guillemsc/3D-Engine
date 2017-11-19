#include "Explorer.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "JSONLoader.h"
#include "Functions.h"
#include "ResourceTextureLoader.h"
#include "ResourceManager.h"

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
	tga_icon = loader.LoadTexture("UI/TGA_icon.png");
	folder_icon = loader.LoadTexture("UI/FOLDER_icon.png");

	App->file_system->SetLookingPath(App->file_system->GetAssetsPath().c_str());
}

void Explorer::Draw()
{
	igBeginDock("Explorer", &visible, ImGuiWindowFlags_MenuBar);	

	if (visible)
	{
		string looking_path = App->file_system->GetLookingPath();

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::MenuItem("Back"))
			{
				if (looking_path != App->file_system->GetAssetsPath())
					App->file_system->SetLookingPath(GetParentDirectory(looking_path));
			}

			if (ImGui::MenuItem("New Folder"))
			{
				App->file_system->CreateFolder(looking_path.c_str(), "new_folder");
			}

			ImGui::EndMenuBar();
		}

		vector<string> files = App->file_system->GetFilesInPath(looking_path.c_str());
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

			if (TextCmp(extension.c_str(), "meta") || TextCmp(extension.c_str(), "prefab"))
				continue;


			if (TextCmp(extension.c_str(), "fbx")) {
				ImGui::ImageButtonWithTextDOWN((ImTextureID*)fbx_icon, filename.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
				
				if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
					App->resource_manager->LoadFileIntoScene((*it).c_str());
			}
			else if (TextCmp(extension.c_str(), "png")) 
			{
				ImGui::ImageButtonWithTextDOWN((ImTextureID*)png_icon, filename.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
			}
			else if (TextCmp(extension.c_str(), "tga")) 
			{
				ImGui::ImageButtonWithTextDOWN((ImTextureID*)tga_icon, filename.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);
			}
			else if (TextCmp(extension.c_str(), ""))
			{
				ImGui::ImageButtonWithTextDOWN((ImTextureID*)folder_icon, name.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);

				if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
				{
					App->file_system->SetLookingPath(path + filename + "\\");
				}
			}
			
			ImGui::PushID(name.c_str());
			if (ImGui::BeginPopupContextItem("HerarchyPopup"))
			{
				if (ImGui::Button("Delete"))
				{
					to_delete = true;
					path_delete = path;
					filename_delete = filename;
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();

		if (i < MAX_FILES_HORIZONTAL)
			ImGui::SameLine();
		else
			i = -1;
		}
	}

	if (to_delete)
	{
		DeleteFileInAssets(path_delete, filename_delete);
	}

	igEndDock();
}

void Explorer::CleanUp()
{
	ResourceTextureLoader loader;
	loader.UnloadTexture(fbx_icon);
	loader.UnloadTexture(png_icon);
	loader.UnloadTexture(tga_icon);
	loader.UnloadTexture(folder_icon);
}

vector<string> Explorer::OrderFiles(vector<string> files)
{
	int size = files.size();
	vector<string> tmp;
	
	int i = 0;
	while (i <= NUM_EXTENSIONS)
	{
		for (vector<string>::iterator it = files.begin(); it != files.end();)
		{
			string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
			string extension = App->file_system->GetFileExtension(filename.c_str());
			extension = ToLowerCase(extension);

			if (i != NUM_EXTENSIONS && (TextCmp(filename.c_str(), ".") || TextCmp(filename.c_str(), "..")))
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
			case 3: // Png
				if (!TextCmp(extension.c_str(), "tga"))
				{
					it++;
					continue;
				}
				break;
			case 4: // Others
				break;
			}
			
			tmp.push_back(*it);
			it = files.erase(it);
		
		}

		i++;
	}

	return tmp;
}

string Explorer::GetParentDirectory(string child)
{
	string parent;
	
	parent = child.substr(0, child.find_last_of("\\"));
	parent = parent.substr(0, parent.find_last_of("\\"));
	parent += "\\";

	return parent;
}

void Explorer::DeleteFileInAssets(string path, string filename)
{
	if (ImGui::BeginPopupModal("Delete File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Do you want to enable static for all the child objects as well?");
		ImGui::Separator();

		if (ImGui::Button("Yes, change children", ImVec2(160, 0)))
		{
			string file_to_delete = path + filename;
			string meta_to_delete = file_to_delete + ".meta";
			remove(file_to_delete.c_str());
			remove(meta_to_delete.c_str());

			to_delete = false;
			path_delete.clear();
			filename_delete.clear();

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(130, 0)))
		{
			to_delete = false;
			path_delete.clear();
			filename_delete.clear();

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
