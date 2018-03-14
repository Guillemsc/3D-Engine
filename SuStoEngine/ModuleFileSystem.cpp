#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <ctype.h>
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

DecomposedFilePath FileSystem::DecomposeFilePath(std::string file_path)
{
	DecomposedFilePath ret;

	bool adding_file_extension = false;
	bool adding_file_name = false;
	int last_bar_pos = 0;

	for (int i = 0; i < file_path.length(); ++i)
	{
		char curr_word = file_path[i];

		// Formating --------------------

		if (curr_word == '/')
			curr_word = '\\';

		ret.file_path += curr_word;

		// ------------------------------

		// File extension ---------------
		if (adding_file_extension)
			ret.file_extension += curr_word;

		if (curr_word == '.')
		{
			adding_file_extension = true;
			ret.file_extension.clear();
		}
		// -------------------------------

		// File name ---------------------

		if (curr_word == '.')
		{
			adding_file_name = false;
		}

		if (adding_file_name)
			ret.file_name += curr_word;

		if (curr_word == '\\')
		{
			last_bar_pos = i;
			adding_file_name = true;
			ret.file_name.clear();
		}

		// -------------------------------
	}

	ret.file_extension_lower_case = ToLowerCase(ret.file_extension);

	// Path ---------------------

	for (int i = 0; i <= last_bar_pos; i++)
	{
		ret.path += ret.file_path[i];
	}

	// --------------------------

	return ret;
}

std::string FileSystem::NewNameForFileNameCollision(const char * filename)
{
	string ret;

	int number = GetFileNameNumber(filename);

	if (number != -1)
	{
		ret = SetFileNameNumber(filename, number + 1);
	}
	else
	{
		ret = SetFileNameNumber(filename, 1);
	}

	return ret;
}

int FileSystem::GetFileNameNumber(const char * filename)
{
	int ret = -1;

	std::string name = filename;

	std::string number_str;
	bool adding = false;

	for (int i = 0; i < name.size(); ++i)
	{
		if (name[i] == '(')
		{
			adding = true;
			number_str.clear();
			continue;
		}

		if (name[i] == ')')
		{
			adding = false;
		}

		if (adding && isdigit(name[i]))
		{
			number_str += name[i];
		}
	}

	if (number_str.size() > 0)
	{
		ret = atoi(number_str.c_str());
	}

	return ret;
}

std::string FileSystem::SetFileNameNumber(const char * filename, int number)
{
	std::string ret = filename;

	if (GetFileNameNumber(filename) != -1)
	{
		int start = 0;
		bool has_start = false;
		bool has_end = false;
		int end = 0;

		for (int i = ret.size() - 1; i >= 0; --i)
		{
			if (ret[i] == ')')
			{
				start = i;
				has_start = true;
			}

			if (ret[i] == '(' && has_start)
			{
				end = i;
				has_end = true;
				break;
			}
		}

		if (has_start && has_end)
			ret = ret.substr(0, end);
	}

	ret += ('(' + std::to_string(number) + ')');

	return ret;
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

std::string FileSystem::GetFilenameWithoutExtension(const char * file_name)
{
	string ret;

	for (int i = 0; file_name[i] != '\0'; i++)
	{
		if (file_name[i] == '.')
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
		if (file_path[i] == '\\')
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
	
	CopyFile(filepath, path.c_str(), false);
	
	DWORD error = GetLastError();

	if(error != 0)
		LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
}

void FileSystem::FileCopyPasteWithNewName(const char * filepath, const char * new_path, const char * new_name)
{
	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	std::string changed_original_filepath = d_filepath.path + new_name + "." + d_filepath.file_extension;

	if (FileRename(filepath, new_name))
	{
		FileCopyPaste(changed_original_filepath.c_str(), new_path);

		FileRename(changed_original_filepath.c_str(), d_filepath.file_name.c_str());
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

bool FileSystem::FileExists(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath d_filepath = DecomposeFilePath(filepath);

	ret = FileExists(d_filepath.file_extension.c_str(), d_filepath.file_name.c_str(), d_filepath.file_extension.c_str());

	return ret;
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



