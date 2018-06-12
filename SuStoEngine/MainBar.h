#ifndef __MAIN_BAR_H__
#define __MAIN_BAR_H__

#include "EditorUI.h"
#include "Module.h"

class MainBar : public EditorElement
{
public:
	MainBar(bool start_enabled = true);
	virtual ~MainBar();

	void Start();
	void Draw(uint flags);

public:

private:
	bool show_imgui_test_window = false;
	char new_layout_name[255];

};

#endif