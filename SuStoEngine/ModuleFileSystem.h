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

	string GetAssetsPath();
	string GetLibraryPath();

private:

public:
	string assets_path;
	string library_path;

};

#endif // __FILESYSTEM_H__