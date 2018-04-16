




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
	App->file_system->SetLookingPath(App->file_system->GetAssetsPath().c_str());
}

void Explorer::Draw()
{
	ImGui::BeginDock("Explorer", false, &visible, false, ImGuiWindowFlags_MenuBar);	
	
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

	Folder folders = App->file_system->GetFilesAndFoldersTree(looking_path.c_str());

		// Options ----------
	//ImGui::PushID(name.c_str());
	//if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	//{
	//	if (ImGui::Button("Load"))
	//	{
	//		App->resource_manager->LoadAssetResourceIntoScene((*it).c_str());
	//	}
	//	if (ImGui::Button("Delete"))
	//	{
	//		path_delete = (*it).c_str();
	//		App->resource_manager->UnloadAssetFromEngine(path_delete.c_str());
	//		ImGui::OpenPopup("Delete PopUp");
	//	}
	//	if (ImGui::Button("Rename"))
	//	{

	//	}
	//	if (ImGui::Button("Show in Explorer"))
	//	{
	//		App->GoToFolder(path.c_str());
	//	}
	//	if (ImGui::Button("Open"))
	//	{
	//		App->GoToFolder((*it).c_str());
	//	}
	//	ImGui::EndPopup();
	//}
	//ImGui::PopID();
	// -------------------

	//if (ImGui::BeginPopupModal("Delete PopUp", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	//{
	//	ImGui::Text("Are you sure you want to delete this file?");
	//	ImGui::Separator();

	//	if (ImGui::Button("Yes, I'm sure", ImVec2(160, 0)))
	//	{
	//		App->resource_manager->UnloadAssetFromEngine(path_delete.c_str());
	//		ImGui::CloseCurrentPopup();
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("No, cancel this action", ImVec2(160, 0)))
	//	{
	//		ImGui::CloseCurrentPopup();
	//	}
	//	ImGui::EndPopup();
	//}
	

	ImGui::EndDock();
}

//void Explorer::CleanUp()
//{
////	ResourceTextureLoader loader;
////	loader.UnloadTexture(fbx_icon);
////	loader.UnloadTexture(png_icon);
////	loader.UnloadTexture(tga_icon);
////	loader.UnloadTexture(folder_icon);
//}

//vector<string> Explorer::OrderFiles(vector<string> files)
//{
//	int size = files.size();
//	vector<string> tmp;
	//
	//int i = 0;
	//while (i <= NUM_EXTENSIONS)
	//{
	//	for (vector<string>::iterator it = files.begin(); it != files.end();)
	//	{
	//		string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
	//		string extension = App->file_system->GetFileExtension(filename.c_str());
	//		extension = ToLowerCase(extension);

	//		if (i != NUM_EXTENSIONS && (TextCmp(filename.c_str(), ".") || TextCmp(filename.c_str(), "..")))
	//		{
	//			it++;
	//			continue;
	//		}

	//		switch (i)
	//		{
	//		case 0: // Folders
	//			if (!TextCmp(extension.c_str(), ""))
	//			{
	//				it++;
	//				continue;
	//			}
	//			break;
	//		case 1: // Fbx
	//			if (!TextCmp(extension.c_str(),  "fbx"))
	//			{
	//				it++;
	//				continue;
	//			}
	//			break;
	//		case 2: // Png
	//			if (!TextCmp(extension.c_str(), "png"))
	//			{
	//				it++;
	//				continue;
	//			}
	//			break;
	//		case 3: // Png
	//			if (!TextCmp(extension.c_str(), "tga"))
	//			{
	//				it++;
	//				continue;
	//			}
	//			break;
	//		case 4: // Others
	//			break;
	//		}
	//		
	//		tmp.push_back(*it);
	//		it = files.erase(it);
	//	
	//	}

	//	i++;
	//}

//	return tmp;
//}

string Explorer::GetParentDirectory(string child)
{
	string parent;
	
	parent = child.substr(0, child.find_last_of("\\"));
	parent = parent.substr(0, parent.find_last_of("\\"));
	parent += "\\";

	return parent;
}

//void Explorer::DeleteFileInAssets(string path, string filename)
//{
//	if (ImGui::BeginPopupModal("Delete File", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
//	{
//		ImGui::Text("Do you want to enable static for all the child objects as well?");
//		ImGui::Separator();
//
//		if (ImGui::Button("Yes, change children", ImVec2(160, 0)))
//		{
//			string file_to_delete = path + filename;
//			string meta_to_delete = file_to_delete + ".meta";
//			remove(file_to_delete.c_str());
//			remove(meta_to_delete.c_str());
//
//			to_delete = false;
//			path_delete.clear();
//			filename_delete.clear();
//
//			ImGui::CloseCurrentPopup();
//		}
//		ImGui::SameLine();
//		if (ImGui::Button("Cancel", ImVec2(130, 0)))
//		{
//			to_delete = false;
//			path_delete.clear();
//			filename_delete.clear();
//
//			ImGui::CloseCurrentPopup();
//		}
//		ImGui::EndPopup();
//	}
//}
