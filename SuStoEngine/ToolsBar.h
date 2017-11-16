#ifndef __TOOLS_BAR_H__
#define __TOOLS_BAR_H__

#include "EditorUI.h"
#include "Module.h"

class ToolsBar : public EditorElement
{
public:
	ToolsBar(bool start_enabled = true);
	virtual ~ToolsBar();

	void Start();
	void Draw();

public:

private:
	bool draw_kdtree = false;
	bool draw_bboxes = false;
};

#endif