#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include <fstream>
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
	path_ex += "*.";
	path_ex += extension;

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
