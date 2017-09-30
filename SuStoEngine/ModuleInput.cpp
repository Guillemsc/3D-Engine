#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "EditorUI.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	SetName("Input");
	keyboard = new KeyBinding[MAX_KEYS];
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Init SDL input event system");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_OUTPUT("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate()
{
	bool ret = true;

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	active_keys.clear();

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		keyboard[i].key = i;

		if (keys[i] == 1)
		{
			if (keyboard[i].state == KEY_IDLE)
			{
				keyboard[i].state = KEY_DOWN;
				AddKey(keyboard[i]);
			}
			else
			{
				keyboard[i].state = KEY_REPEAT;
				AddKey(keyboard[i]);
			}
		}
		else
		{
			if (keyboard[i].state == KEY_REPEAT || keyboard[i].state == KEY_DOWN)
			{
				keyboard[i].state = KEY_UP;
				AddKey(keyboard[i]);
			}
			else
			{
				keyboard[i].state = KEY_IDLE;
			}
		}
	}

Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

mouse_x /= SCREEN_SIZE;
mouse_y /= SCREEN_SIZE;
mouse_z = 0;

for (int i = 0; i < 5; ++i)
{
	if (buttons & SDL_BUTTON(i))
	{
		if (mouse_buttons[i] == KEY_IDLE)
			mouse_buttons[i] = KEY_DOWN;
		else
			mouse_buttons[i] = KEY_REPEAT;
	}
	else
	{
		if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_UP;
		else
			mouse_buttons[i] = KEY_IDLE;
	}
}

mouse_x_motion = mouse_y_motion = 0;

bool quit = false;
SDL_Event e;
while (SDL_PollEvent(&e))
{
	App->editorUI->ImGuiInput(&e);

	switch (e.type)
	{
	case SDL_QUIT:
		windowEvents[WE_QUIT] = true;
		break;

	case SDL_WINDOWEVENT:
		switch (e.window.event)
		{
			//case SDL_WINDOWEVENT_LEAVE:
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
		case SDL_WINDOWEVENT_FOCUS_LOST:
			windowEvents[WE_HIDE] = true;
			break;

			//case SDL_WINDOWEVENT_ENTER:
		case SDL_WINDOWEVENT_SHOWN:
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			windowEvents[WE_SHOW] = true;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			App->window->SetWindowSize(e.window.data1, e.window.data2);
			break;
		}

		break;

	case SDL_MOUSEWHEEL:
		mouse_z = e.wheel.y;
		break;

	case SDL_MOUSEMOTION:
		mouse_x = e.motion.x / SCREEN_SIZE;
		mouse_y = e.motion.y / SCREEN_SIZE;

		mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
		mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
		break;
	}
}

return ret;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return ret;
}

// char -> SDL_GetScancode: https://wiki.libsdl.org/SDL_Keycode
int ModuleInput::CharToKey(const char * key)
{
	return SDL_GetScancodeFromKey(SDL_GetKeyFromName(key));
}

void ModuleInput::AddKey(KeyBinding k)
{
	for(vector<KeyBinding>::iterator it = active_keys.begin(); it != active_keys.end(); it++)
	{
		if ((*it) == k)
		{
			return;
		}
	}

	active_keys.push_back(k);
}

void ModuleInput::RemoveKey(KeyBinding k)
{
	for (vector<KeyBinding>::iterator it = active_keys.begin(); it != active_keys.end(); it++)
	{
		if ((*it) == k)
		{
			active_keys.erase(it);
			break;
		}
	}
}

KEY_STATE ModuleInput::GetKeyBinding(const char * binding)
{
	for (vector<KeyBinding>::iterator it = active_keys.begin(); it != active_keys.end(); it++)
	{
		if (TextCmp((*it).binding_name, binding))
		{
			return (*it).state;
		}
	}

	return KEY_STATE::KEY_IDLE;
}

void ModuleInput::SetKeyBinding(const char * key, const char * binding_name)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (i == CharToKey(key))
		{
			keyboard[i].binding_name = binding_name;
			break;
		}
	}
}

bool ModuleInput::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}
