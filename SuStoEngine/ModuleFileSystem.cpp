#include "ModuleFileSystem.h"
#include "App.h"
#include <Windows.h>
#include "Functions.h"

FileSystem::FileSystem(bool start_enabled)
{
	SetName("File System");

	assets_path = CreateFolder(App->GetBasePath(), "Assets");
	library_path = CreateFolder(App->GetBasePath(), "Library");
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

string FileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string filepath = path; 
	
	if (filepath[filepath.length()-1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_ALREADY_EXISTS)
		{
			LOG_OUTPUT("Error creating folder (Folder aleady exists): %s", filepath.c_str())
			ret = filepath;
		}
		else if (error == ERROR_PATH_NOT_FOUND)
		{
			LOG_OUTPUT("Error creating folder (Path not found): %s", path)
		}
	}
	else
		ret = filepath;

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

void FileSystem::FileCopyPaste(const char * filepath, const char * new_path, bool replace_existing)
{
	string path = new_path;

	if (path[path.length() - 1] != '\\')
	{
		path += '\\';
	}

	path += GetFileNameFromFilePath(filepath);

	if (!replace_existing)
	{
		if (CopyFile(filepath, path.c_str(), false))
		{
			LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}
	}
	else
	{
	/*	if (CopyFileEx(filepath, path.c_str(),COPYFILE_REPLACE))
		{
			LOG_OUTPUT("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}*/
	}
}
