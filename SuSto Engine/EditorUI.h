#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"

class EditorUI : public Module
{
public:
	EditorUI(bool enabled = true);

	// Destructor
	virtual ~EditorUI();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	void ImGuiInput(SDL_Event* ev);

public:

private:

};

#endif // __j1FILESYSTEM_H__