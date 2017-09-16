#pragma once

#include "Module.h"


class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool CleanUp();

	void ImGuiEvent(SDL_Event* ev);

	void NewFrame();
	void Draw();

private:

};