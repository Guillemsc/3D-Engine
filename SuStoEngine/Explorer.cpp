




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
	//ResourceTextureLoader loader;
	//fbx_icon = loader.LoadTexture("UI\\FBX_icon.png").id;
	//png_icon = loader.LoadTexture("UI\\PNG_icon.png").id;
	//tga_icon = loader.LoadTexture("UI\\TGA_icon.png").id;
	//folder_icon = loader.LoadTexture("UI\\FOLDER_icon.png").id;

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

	vector<string> files = App->file_system->GetFilesInPath(looking_path.c_str());
	//files = OrderFiles(files);

	int i = 0;
	for (vector<string>::iterator it = files.begin(); it != files.end(); ++it)
	{
	//	string path = App->file_system->GetPathFromFilePath((*it).c_str());
	//	string filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
	//	string extension = App->file_system->GetFileExtension(filename.c_str());
	//	extension = ToLowerCase(extension);
	//	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str(), false);

	//	if (TextCmp(filename.c_str(), ".") || TextCmp(filename.c_str(), "..") || TextCmp(extension.c_str(), "meta") || TextCmp(extension.c_str(), "prefab"))
	//		continue;

	//	if (TextCmp(extension.c_str(), "fbx"))
	//		type = ExtensionType::FBX;
	//	else if (TextCmp(extension.c_str(), "png"))
	//		type = ExtensionType::PNG;
	//	else if (TextCmp(extension.c_str(), "tga"))
	//		type = ExtensionType::TGA;
	//	else
	//		type = ExtensionType::FOLDER;
	//	
	//	// set current icon
	//	switch (type)
	//	{
	//	case FOLDER:
	//		current_icon = folder_icon;
	//		break;
	//	case FBX:
	//		current_icon = fbx_icon;
	//		break;
	//	case PNG:
	//		current_icon = png_icon;
	//		break;
	//	case TGA:
	//		current_icon = tga_icon;
	//		break;
	//	}

	//	ImGui::ImageButtonWithTextDOWN((ImTextureID*)current_icon, filename.c_str(), ImVec2(50, 50), ImVec2(-1, 1), ImVec2(0, 0), 10);

	//	// Actions with the icons
	//	switch (type)
	//	{
	//	case FOLDER:
	//		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
	//			App->file_system->SetLookingPath(path + filename + "\\");

	//		break;
	//	case FBX:
	//		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
	//			App->resource_manager->LoadFileIntoScene((*it).c_str());

	//		break;
	//	case PNG:

	//		break;
	//	case TGA:

	//		break;
	//	}

	//	// Options ----------
	//	ImGui::PushID(name.c_str());
	//	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	//	{
	//		if (ImGui::Button("Load"))
	//		{
	//			App->resource_manager->LoadFileIntoScene((*it).c_str());
	//		}
	//		if (ImGui::Button("Delete"))
	//		{
	//			path_delete = path;
	//			App->resource_manager->DeImportFile((*it).c_str());
	//			ImGui::OpenPopup("Delete PopUp");
	//		}
	//		if (ImGui::Button("Rename"))
	//		{
	//			path_delete = path;
	//			App->resource_manager->DeImportFile((*it).c_str());
	//			ImGui::OpenPopup("Delete PopUp");
	//		}
	//		if (ImGui::Button("Show in Explorer"))
	//		{
	//			App->GoToFolder(path.c_str());
	//		}
	//		if (ImGui::Button("Open"))
	//		{
	//			App->GoToFolder((*it).c_str());
	//		}
	//		ImGui::EndPopup();
	//	}
	//	ImGui::PopID();
	//	// -------------------

	//if (i < MAX_FILES_HORIZONTAL)
	//	ImGui::SameLine();
	//else
	//	i = -1;
	}

	if (ImGui::BeginPopupModal("Delete PopUp", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Text("Are you sure you want to delete this file?");
		ImGui::Separator();

		if (ImGui::Button("Yes, I'm sure", ImVec2(160, 0)))
		{
			//App->resource_manager->DeImportFile(path_delete.c_str());
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("No, cancel this action", ImVec2(160, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	

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
