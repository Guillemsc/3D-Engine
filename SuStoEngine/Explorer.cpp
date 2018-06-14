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
	looking_timer.Start();
	update_folders = true;
}

void Explorer::Draw(uint flags)
{
	flags |= ImGuiWindowFlags_MenuBar;

	if (looking_timer.ReadSec() > 2.0f)
	{
		looking_timer.Start();
		update_folders = true;
	}

	ImGui::BeginDock("Explorer", false, &visible, false, flags);

	ImGui::Columns(2);

	if (update_folders)
	{
		folders = App->file_system->GetFilesAndFoldersTree(App->file_system->GetAssetsPath().c_str());
		update_folders = false;
	}

	DrawFoldersRecursive(folders);

	ImGui::NextColumn();

	DrawFiles(looking_folder);
	
	ImGui::EndDock();
}

void Explorer::UpdateFloders()
{
	update_folders = true;
}

void Explorer::AddToSelectedFiles(std::string file)
{
	bool found = false;

	if (App->file_system->FileExists(file.c_str()))
	{
		for (std::vector<std::string>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
		{
			if ((*it) == file)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			selected_files.push_back(file);
		}
	}
}

void Explorer::RemoveFromSelectedFiles(std::string file)
{
	for (std::vector<std::string>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
	{
		if ((*it) == file)
		{
			selected_files.erase(it);
			break;
		}
	}
}

void Explorer::ClearSelectedFiles()
{
	selected_files.clear();
}

void Explorer::DrawFoldersRecursive(const Folder& folder)
{
	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (folder.folder_path == App->file_system->GetAssetsPath().c_str())
	{
		ImGui::SetNextTreeNodeOpen(true);
	}

	if (looking_folder.valid && looking_folder.folder_path == folder.folder_path)
	{
		flags |= ImGuiTreeNodeFlags_Selected;

		// Update looking folder files and paths
		looking_folder = folder;
	}

	if (folder.folders.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	ImGui::PushID(folder.folder_path.c_str());
	bool opened = ImGui::TreeNodeEx(folder.folder_name.c_str(), flags);

	if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
	{
		looking_folder = folder;

		if (ImGui::IsItemClicked(1))
		{
			ImGui::OpenPopup("SelectedFolderPopup");
		}
	}

	if (looking_folder.valid && looking_folder.folder_path == folder.folder_path)
	{
		if (ImGui::BeginPopup("SelectedFolderPopup"))
		{
			if (ImGui::Button("Create folder"))
			{
				App->file_system->CreateFolder(folder.folder_path.c_str(), "NewFolder");
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Rename"))
			{
				ImGui::OpenPopup("RenameFolderPopup");
			}

			if (ImGui::Button("Show in Explorer"))
			{
				App->GoToFolder(folder.folder_path.c_str());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Delete"))
			{
				App->file_system->FolderDelete(folder.folder_path.c_str());
				ImGui::CloseCurrentPopup();
			}

			ImGui::Separator();

			if (ImGui::Button("Import new Asset"))
			{

			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("RenameFolderPopup"))
		{
			char name[100];
			strcpy_s(name, 100, rename_folder_tmp.c_str());
			if (ImGui::InputText("Factory object name", name, 100))
			{
				rename_folder_tmp = name;
			}
			if (ImGui::Button("Save"))
			{
				App->file_system->FolderRename(folder.folder_path.c_str(), rename_folder_tmp.c_str());
				UpdateFloders();

				rename_folder_tmp = "";
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	ImGui::PopID();

	if (opened)
	{
		for (std::vector<Folder>::const_iterator it = folder.folders.begin(); it != folder.folders.end(); ++it)
		{
			DrawFoldersRecursive(*it);
		}

		ImGui::TreePop();
	}
}

void Explorer::DrawFiles(const Folder& folder)
{
	if (folder.valid)
	{
		for (std::vector<DecomposedFilePath>::const_iterator it = folder.files.begin(); it != folder.files.end(); ++it)
		{
			if ((*it).file_extension != "meta")
			{
				ImGui::PushID((*it).file_path.c_str());

				ImGui::Selectable((*it).file_name.c_str());

				if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1))
				{
					if (ImGui::IsItemClicked(1))
					{
						AddToSelectedFiles((*it).file_path);
						ImGui::OpenPopup("SelectedFilePopup");
					}
				}

				if (ImGui::BeginPopup("SelectedFilePopup"))
				{
					if (ImGui::Button("Load"))
					{
						App->resource_manager->LoadAssetResourceIntoScene((*it).file_path.c_str());
					}

					if (ImGui::Button("Delete"))
					{
						App->resource_manager->UnloadAssetFromEngine((*it).file_path.c_str());
					}

					ImGui::EndPopup();
				}

				ImGui::PopID();
			}
		}
	}
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

//string Explorer::GetParentDirectory(string child)
//{
//	string parent;
//	
//	parent = child.substr(0, child.find_last_of("\\"));
//	parent = parent.substr(0, parent.find_last_of("\\"));
//	parent += "\\";
//
//	return parent;
//}

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
