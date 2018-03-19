#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "Module.h"
#include <filesystem>

struct DecomposedFilePath
{
	std::string file_name;
	std::string file_extension;
	std::string file_extension_lower_case;
	std::string path;
	std::string file_path;
};

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
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = false);
	bool FileCopyPaste(const char* filepath, const char* new_path, std::string &resultant_path);
	bool FileCopyPaste(const char* filepath, const char* new_path);
	void FileCopyPasteWithNewName(const char* filepath, const char* new_path, const char* new_name);
	bool FileDelete(const char* filepath);
	bool FileSave(const char * path, const char * file_content, const char * name, const char * extension, int size);
	vector<string> GetFilesInPath(const char* path, const char* extension = "");
	bool FileExists(const char* path, const char* name, const char* extension = "");
	bool FileExists(const char* filepath);
	bool FileRename(const char* filepath, const char* new_name);	
	bool FolderExists(const char* path);

	string GetAssetsPath();
	string GetLibraryPath();
	string GetLibraryMeshPath();
	string GetLibraryTexturePath();
	string GetLibraryScenePath();
	string GetSettingsPath();

	std::string GetLookingPath();
	void SetLookingPath(const string & new_path);

	DecomposedFilePath DecomposeFilePath(std::string file_path);

	std::string NewNameForFileNameCollision(const char* filename);
	int GetFileNameNumber(const char* filename);
	std::string SetFileNameNumber(const char* filename, int number);

	// Example file.ex -> .ex
	std::string GetFileExtension(const char* file_name);

	// Example file.ex -> file
	std::string GetFilenameWithoutExtension(const char* file_name);

	// Example C:/user/folder/file.ex -> file.ex
	std::string GetFileNameFromFilePath(const char* file_path);

	// Example C:/user/folder/file.ex -> C:/user/folder/
	std::string GetPathFromFilePath(const char* file_path);

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