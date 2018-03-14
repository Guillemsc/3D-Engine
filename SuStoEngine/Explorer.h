#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include "EditorUI.h"
#include "Module.h"
#include <vector>

#define SPACING 10
#define MAX_FILES_HORIZONTAL 8
#define NUM_EXTENSIONS 4

class Resource;

enum ExtensionType
{
	FOLDER,
	FBX,
	PNG,
	TGA,

	NO_TYPE
};

class Explorer : public EditorElement
{
public:
	Explorer(bool start_enabled = true);
	virtual ~Explorer();

	void Start();
	void Draw();
	//void CleanUp();

	//vector<string> OrderFiles(vector<string> files);
	string GetParentDirectory(string child);
	//void DeleteFileInAssets(string path, string filename);

public:

private:
	uint fbx_icon = 0;
	uint png_icon = 0;
	uint folder_icon = 0;
	uint tga_icon = 0;

	uint current_icon = 0;

	string path_delete;

	std::vector<Resource*> resources;

	ExtensionType type = ExtensionType::NO_TYPE;
};

#endif