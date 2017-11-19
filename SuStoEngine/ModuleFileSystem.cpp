#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include "Functions.h"

FileSystem::FileSystem(bool start_enabled)
{
	SetName("FileSystem");

	assets_path = CreateFolder(App->GetBasePath(), "Assets");
	library_path = CreateFolder(App->GetBasePath(), "Library");
	library_mesh_path = CreateFolder(library_path.c_str(), "Meshes");
	library_texture_path = CreateFolder(library_path.c_str(), "Textures");
	library_scene_path = CreateFolder(library_path.c_str(), "Scenes");
	settings_path = CreateFolder(App->GetBasePath(), "Settings");
}

FileSystem::~FileSystem()
{
}

bool FileSystem::Awake()
{
	bool ret = true;

	return ret;
}

bool FileSystem::Start()
{
	bool ret = true;

	return ret;
}

bool FileSystem::Update()
{
	bool ret = true;

	return ret;
}

bool FileSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

string FileSystem::GetAssetsPath()
{
	return assets_path;
}

string FileSystem::GetLibraryPath()
{
	return library_path;
}

string FileSystem::GetLibraryMeshPath()
{
	return library_mesh_path;
}

string FileSystem::GetLibraryTexturePath()
{
	return library_texture_path;
}

string FileSystem::GetLibraryScenePath()
{
	return library_scene_path;
}

string FileSystem::GetSettingsPath()
{
	return settings_path;
}

string FileSystem::GetLookingPath()
{
	return looking_path;
}

void FileSystem::SetLookingPath(const string& new_path)
{
	looking_path = new_path;
}

std::string FileSystem::GetFileExtension(const char * file_name)
{
	string ret;

	bool adding = false;
	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
		{
			ret.clear();
			adding = true;
			continue;
		}

		if (adding)
			ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFilenameWithoutExtension(const char * file_name, bool without_)
{
	string ret;

	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.' || (without_ && file_name[i] == '_'))
		{
			break;
		}

		ret += file_name[i];
	}

	return ret;
}

std::string FileSystem::GetFileNameFromFilePath(const char * file_path)
{
	string ret;

	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += file_path[i];
	}

	return ret;
}

std::string FileSystem::GetPathFromFilePath(const char * file_path)
{
	string ret;

	int last = 0;
	for (int i = 0; file_path[i] != '\0'; i++)
	{
		if (file_path[i] == '\\' || file_path[i] == '/')
		{
			last = i;
			last++;
		}
	}

	for (int i = 0; i < last && file_path[i] != '\0'; i++)
	{
		ret += file_path[i];
	}

	return ret;
}

string FileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string filepath = path; 
	
	if (filepath[filepath.length()-1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		LOG_OUTPUT("Error creating folder (path not found): %s", path);
		return ret;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		LOG_OUTPUT("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '\\';

	return ret;
}

void FileSystem::FileMove(const char * filepath, const char * new_path, bool replace_existing)
{
	string path = new_path;

	if (path[path.length()-1] != '\\')
	{
		path += '\\';
	}

	path += GetFileNameFromFilePath(filepath);

	if (!replace_existing)
	{
		if (MoveFile(filepath, path.c_str()))
		{
			LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}
	}
	else
	{
		if (MoveFileEx(filepath, path.c_str(), MOVEFILE_REPLACE_EXISTING))
		{
			LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}
	}
}

void FileSystem::FileCopyPaste(const char * filepath, const char * new_path)
{
	string path = new_path;

	if (path[path.length() - 1] != '\\')
	{
		path += '\\';
	}

	path += GetFileNameFromFilePath(filepath);
	
	if (CopyFile(filepath, path.c_str(), false))
	{
		LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
	}
	
}

void FileSystem::FileDelete(const char * filepath)
{
	if (DeleteFile(filepath) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			LOG_OUTPUT("Error deleting file (path not found)): %s", filepath);
		}
	}
}

bool FileSystem::FileSave(const char * path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else 
	{
		LOG_OUTPUT("Error saving file %s: ", name);
	}

	if(new_file)
		fclose(new_file);

	return ret;
}

vector<string> FileSystem::GetFilesInPath(const char * path, const char* extension)
{
	vector<string> files;

	WIN32_FIND_DATA search_data;

	string path_ex = path;

	if (!TextCmp(extension, ""))
	{
		path_ex += "*.";
		path_ex += extension;
	}
	else
	{
		path_ex += "*.*";
	}

	HANDLE handle = FindFirstFile(path_ex.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		string path_new = path;
		path_new += search_data.cFileName;
		files.push_back(path_new);

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	if(handle)
		FindClose(handle);

	return files;
}

bool FileSystem::FileExists(const char * path, const char * name, const char * extension)
{
	WIN32_FIND_DATA search_data;

	bool has_extension = true;
	if (TextCmp(extension, ""))
		has_extension = false;

	string filepath = path;
	if (has_extension)
	{
		filepath += "*.";
		filepath += extension;
	}
	else
	{
		filepath += "*.*";
	}

	string filename = name;
	if (has_extension)
	{
		filename += ".";
		filename += extension;
	}

	HANDLE handle = FindFirstFile(filepath.c_str(), &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		string found_file = search_data.cFileName;

		if(!has_extension)
			found_file = GetFilenameWithoutExtension(search_data.cFileName);
		
		if (TextCmp(found_file.c_str(), filename.c_str()))
			return true;
		
		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	return false;
}

bool FileSystem::FileRename(const char * filepath, const char * new_name)
{
	bool ret = false;

	string path = GetPathFromFilePath(filepath);
	string name = GetFileNameFromFilePath(filepath);
	string extension = GetFileExtension(name.c_str());

	string new_filepath = path + new_name + "." + extension;

	int result;
	result = rename(filepath, new_filepath.c_str());
	if (result == 0)
		ret = true;

	return ret;
}

bool FileSystem::CheckNameCollision(const char * filepath, std::string& new_name)
{
	bool ret = false;

	string last_new_name = new_name;
	new_name.clear();

	string path = App->file_system->GetPathFromFilePath(filepath);
	string filename = App->file_system->GetFileNameFromFilePath(filepath);
	string name = App->file_system->GetFilenameWithoutExtension(filename.c_str());
	string extension = App->file_system->GetFileExtension(filename.c_str());

	vector<std::string> files = App->file_system->GetFilesInPath(path.c_str());

	for (vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		string cmp_filename = App->file_system->GetFileNameFromFilePath((*it).c_str());
		string cmp_name = App->file_system->GetFilenameWithoutExtension(cmp_filename.c_str());

		if (TextCmp(filename.c_str(), cmp_filename.c_str()))
		{
			ret = true;
			string values;
			bool finding = false;
			bool found = false;
			int val = 0;

			// Already has a copy?
			for (int i = cmp_name.length() - 1; i >= 0; --i)
			{
				char ch = cmp_name[i];

				if (i == cmp_name.length() - 1 && !isdigit(ch))
					break;

				if (isdigit(ch))
				{
					values.insert(0, 1, ch);
					finding = true;
					continue;
				}

				if (finding && ch == '-')
				{
					val = atoi(values.c_str());
					found = true;
					break;
				}
			}

			// Add value to copy
			if (found)
			{
				for (int i = cmp_name.length() - 1; i >= 0; --i)
				{
					char ch = cmp_name[i];

					if (isdigit(ch))
					{
						cmp_name.erase(i, 1);
						continue;
					}

					if (finding && ch == '-')
					{
						cmp_name.erase(i, 1);
						break;
					}
				}

				new_name = cmp_name + "-" + std::to_string(val + 1);

				string new_path = path + new_name + "." + extension;

				CheckNameCollision(new_path.c_str(), new_name);

				break;
			}

			// New copy
			else
			{
				new_name = name + "-1";

				string new_path = path + new_name + "." + extension;

				CheckNameCollision(new_path.c_str(), new_name);

				break;
			}
		}
	}

	if (!ret)
		new_name = last_new_name;

	return ret;
}
