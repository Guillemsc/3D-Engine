#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "JSONLoader.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Init SDL window & surface");
	SetName("Window");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	width = 1280;
	height = 1024;
	fullscreen = false;
	resizable = true;
	borderless = false;
	full_dekstop = false;
	maximized = true;
	vsync = false;

	JSON_Doc* config = App->GetConfig();

	if (config != nullptr)
	{
		width = config->GetNumber("window.width");
		height = config->GetNumber("window.height");
		fullscreen = config->GetBool("window.fullscreen");
		resizable = config->GetBool("window.resizable");
		borderless = config->GetBool("window.borderless");
		full_dekstop = config->GetBool("window.fulldekstop");
		maximized = config->GetBool("window.maximized");
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_OUTPUT("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		ret = GenerateWindow(fullscreen, resizable, borderless, full_dekstop, maximized);
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG_OUTPUT("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::GenerateWindow(bool fullscreen, bool resizable, bool borderless, bool full_dekstop, bool maximized)
{
	bool ret = true;

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	if (fullscreen == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	if (resizable == true)
	{
		flags |= SDL_WINDOW_RESIZABLE;
	}

	if (borderless == true)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

	if (full_dekstop == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (maximized == true)
	{
		flags |= SDL_WINDOW_MAXIMIZED;
	}

	window = SDL_CreateWindow(App->GetAppName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

	if (window == NULL)
	{
		LOG_OUTPUT("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		screen_surface = SDL_GetWindowSurface(window);
	}

	return ret;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetWindowSize(int _width, int _height)
{
	if (_width > 0 && _height > 0)
	{
		width = _width;
		height = _height;
		SDL_SetWindowSize(window, width, height);

		if(!fullscreen)
			App->renderer3D->OnResize(width, height);
	}
}

void ModuleWindow::GetWindowSize(int & width, int & height)
{
	SDL_GetWindowSize(window, &width, &height);
}

vec2 ModuleWindow::GetWindowSize()
{
	int w, h;
	GetWindowSize(w, h);
	return vec2(w, h);
}

void ModuleWindow::GetDisplaySize(int & width, int & height)
{
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}

	width = dm.w;
	height = dm.h;
}

void ModuleWindow::SetFullscreen(bool set)
{
	fullscreen = set;

	Uint32 flags;

	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	SDL_SetWindowFullscreen(window, flags);
}

bool ModuleWindow::GetFullscreen()
{
	return fullscreen;
}

void ModuleWindow::SetResizable(bool set)
{

}

bool ModuleWindow::GetResizalbe()
{
	return resizable;
}

void ModuleWindow::SetBorderless(bool set)
{
	if (!fullscreen && !full_dekstop)
	{
		borderless = set;
		SDL_SetWindowBordered(window, (SDL_bool)!set);
	}
}

bool ModuleWindow::GetBorderless()
{
	return borderless;
}

void ModuleWindow::SetFullDekstop(bool set)
{
	full_dekstop = set;

	if (full_dekstop)
	{
		SetFullscreen(false);

		if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			full_dekstop = false;
		}
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}

bool ModuleWindow::GetFullDekstop()
{
	return full_dekstop;
}

void ModuleWindow::SetMaximized(bool set)
{
	maximized = set;
	GenerateWindow(fullscreen, resizable, borderless, full_dekstop, maximized);
}

bool ModuleWindow::GetMaximized()
{
	return maximized;
}

bool ModuleWindow::GetVsync()
{
	return vsync;
}
