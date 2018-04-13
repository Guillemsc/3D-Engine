#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"
#include "glmath.h"
#include "SDL/include/SDL.h"

#define SCREEN_SIZE 1

class Application;
class Cursor;

class ModuleWindow : public Module
{
public:
	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);
	bool Awake();
	bool Update();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetWindowSize(int width, int height);
	void GetWindowSize(int &width, int &height);
	const vec2 GetWindowSize();
	void GetDisplaySize(int &width, int &height);
	const vec2 GetDisplaySize();
	void SetFullscreen(const bool& set);
	const bool GetFullscreen() const;
	void SetResizable(const bool& set);
	const bool GetResizable() const;
	void SetBorderless(const bool& set);
	const bool GetBorderless() const;
	void SetFullDekstop(const bool& set);
	const bool GetFullDekstop() const;
	void SetMaximized(const bool& set);
	const bool GetMaximized();
	void SetBrightness(float& set);
	const float GetBrightness() const;
	void SetVsync(const bool& set);
	const bool GetVsync() const;
	Cursor* GetCursor() const;

//private:
	SDL_Window* GenerateWindow(SDL_Window* window, SDL_Surface* surface, const char* name, Uint32 flags, vec2 pos = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED }, vec2 size = { 500, 500 }, float brightness = 1.0f);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

private:
	Cursor* cursor = nullptr;

	int     width = 0;
	int     height = 0;
	bool    fullscreen = false;
	bool    resizable = false;
	bool    borderless = false;
	bool    full_dekstop = false;
	bool    maximized = true;
	float   brightness = 0.0f;
	bool    vsync = false;
};

#endif // !__MODULE_WINDOW_H__