#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "Module.h"

class FileSystem : public Module
{
public:
	FileSystem(bool start_enabled = true);
	~FileSystem();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = true);
	void FileCopyPaste(const char* filepath, const char* new_path);
	void FileDelete(const char* filepath);
	bool FileSave(const char * path, const char * file_content, const char * name, const char * extension, int size);
	vector<string> GetFilesInPath(const char* path, const char* extension = "");
	bool FileExists(const char* path, const char* name, const char* extension = "");
	bool FileRename(const char* filepath, const char* new_name);

	bool CheckNameCollision(const char* filepath, std::string& new_name);

	string GetAssetsPath();
	string GetLibraryPath();
	string GetLibraryMeshPath();
	string GetLibraryTexturePath();
	string GetLibraryScenePath();
	string GetSettingsPath();

	string GetLookingPath();
	void SetLookingPath(const string & new_path);

	// Example file.ex -> .ex
	std::string GetFileExtension(const char* file_name);

	// Example file.ex -> file
	std::string GetFilenameWithoutExtension(const char* file_name, bool without_ = true);

	// Example C:/user/folder/file.ex -> file.ex
	std::string GetFileNameFromFilePath(const char* file_path);

	// Example C:/user/folder/file.ex -> C:/user/folder/
	std::string GetPathFromFilePath(const char* file_path);

	std::string ProcessFilePath(const char* file_path);

private:

private:
	string assets_path;
	string library_path;
	string library_mesh_path;
	string library_texture_path;
	string library_scene_path;
	string settings_path;

	string looking_path;
};

#endif // __FILESYSTEM_H__