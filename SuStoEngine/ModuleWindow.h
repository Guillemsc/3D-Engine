#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "glmath.h"
#include "SDL/include/SDL.h"

#define SCREEN_SIZE 1

class Application;

class ModuleWindow : public Module
{
public:
	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Awake();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetWindowSize(int width, int height);
	void GetWindowSize(int &width, int &height);
	vec2 GetWindowSize();
	void GetDisplaySize(int &width, int &height);
	void SetFullscreen(bool set);
	bool GetFullscreen();
	void SetResizable(bool set);
	bool GetResizalbe();
	void SetBorderless(bool set);
	bool GetBorderless();
	void SetFullDekstop(bool set);
	bool GetFullDekstop();
	void SetMaximized(bool set);
	bool GetMaximized();
	bool GetVsync();

private:
	bool GenerateWindow(bool fullscreen, bool resizable, bool borderless, bool full_dekstop, bool maximized);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

private:
	int  width = 0;
	int  height = 0;
	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool full_dekstop = false;
	bool maximized = false;
	bool vsync = false;
};

#endif // __ModuleWindow_H__