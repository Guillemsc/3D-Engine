#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include "EditorUI.h"
#include "Module.h"
#include <vector>
#include "ModuleFileSystem.h"
#include "Timer.h"

#define SPACING 10
#define MAX_FILES_HORIZONTAL 8
#define NUM_EXTENSIONS 4

class Resource;

class Explorer : public EditorElement
{
public:
	Explorer(bool start_enabled = true);
	virtual ~Explorer();

	void Start();
	void Draw(uint flags);

	void UpdateFloders();

	void AddToSelectedFiles(std::string file);
	void RemoveFromSelectedFiles(std::string file);
	void ClearSelectedFiles();

private:
	void DrawFoldersRecursive(const Folder& folder);
	void DrawFiles(const Folder& folder);

private:
	Folder folders;
	Folder looking_folder;

	Timer looking_timer;

	bool update_folders = true;

	std::string rename_folder_tmp;

	std::vector<std::string> selected_files;
};

#endif