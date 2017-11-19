#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include "EditorUI.h"
#include "Module.h"
#include <vector>

#define SPACING 10
#define MAX_FILES_HORIZONTAL 5

class Resource;

class Explorer : public EditorElement
{
public:
	Explorer(bool start_enabled = true);
	virtual ~Explorer();

	void Start();
	void Draw();

	vector<string> OrderFiles(vector<string> files);

public:

private:
	uint fbx_icon = 0;
	uint png_icon = 0;
	uint folder_icon = 0;

	std::vector<Resource*> resources;
};

#endif