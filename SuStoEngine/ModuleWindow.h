#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

enum dock_orientation
{
	dock_up,		// 0
	dock_down,		// 1
	dock_left,		// 2
	dock_right,		// 3

	dock_float,		// 4
	dock_null		// 5
};

struct Dock
{
	Dock(char* title);
	void DeleteDock();

	char* title = "";
	SDL_Window* window = nullptr;
	SDL_Surface* screen_surface = nullptr;

	dock_orientation orientation = dock_null;
	int id = 0;
};

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Awake();
	bool CleanUp();

	void SetTitle(const char* title);
	void GetWindowSize(int &width, int &height);

	void CreateNewWindow(char* title);

public:
	list<Dock*> docks;
	int dock_id = 0;

	//The window we'll be rendering to
	SDL_Window* main_window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // __ModuleWindow_H__