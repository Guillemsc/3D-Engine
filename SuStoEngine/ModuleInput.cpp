#include "Globals.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "EditorUI.h"
#include "JSONLoader.h"

// SDL_GetScancode: https://wiki.libsdl.org/SDL_Keycode
// To use PS3 Controller install this driver https://github.com/nefarius/ScpToolkit/releases/tag/v1.6.238.16010

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	SetName("Input");
	keyboard = new KeyBinding[MAX_KEYS];

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i].key = i;
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

void ModuleInput::OnLoadConfig(JSON_Doc* config)
{
	SetKeyBinding(config->GetString("input.console", "º"), "console");
	SetKeyBinding(config->GetString("input.profiler", "p"), "profiler");
	SetKeyBinding(config->GetString("input.configuration", "c"), "configuration");
}

void ModuleInput::OnSaveConfig(JSON_Doc * config)
{
	config->SetString("input.console", GetKeyBinding("console"));
	config->SetString("input.profiler", GetKeyBinding("profiler"));
	config->SetString("input.configuration", GetKeyBinding("configuration"));
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

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG_OUTPUT("Error on SDL_Init_Video");
		ret = false;
	}

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
	{
		LOG_OUTPUT("Error on SDL_Init_GameController");
		ret = false;
	}

	return ret;
}

// Called every draw update
bool ModuleInput::PreUpdate()
{
	bool ret = true;

	SDL_PumpEvents();

	SDL_StartTextInput();
	text_input.clear();

	keys_down.clear();
	keys_repeat.clear();
	keys_up.clear();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i].state == KEY_IDLE)
			{
				keyboard[i].state = KEY_DOWN;
				AddKeyDown(keyboard[i]);
			}
			else
			{
				keyboard[i].state = KEY_REPEAT;
				AddKeyRepeat(keyboard[i]);
			}
		}
		else
		{
			if (keyboard[i].state == KEY_REPEAT || keyboard[i].state == KEY_DOWN)
			{
				keyboard[i].state = KEY_UP;
				AddKeyUp(keyboard[i]);
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

		case SDL_TEXTINPUT:
			text_input.insert(text_input.size(), e.text.text);
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

bool ModuleInput::GetKeyDown(int id)
{
	if (!keys_down.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

bool ModuleInput::GetKeyRepeat(int id)
{
	if (!keys_repeat.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

bool ModuleInput::GetKeyUp(int id)
{
	if (!keys_up.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

bool ModuleInput::GetKeyDown(const char * key)
{
	return GetKeyDown(CharToKey(key));
}

bool ModuleInput::GetKeyRepeat(const char * key)
{
	return GetKeyRepeat(CharToKey(key));
}

bool ModuleInput::GetKeyUp(const char * key)
{
	return GetKeyUp(CharToKey(key));
}

bool ModuleInput::GetKeyBindingDown(const char * binding_name)
{
	if (!keys_down.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}

	return false;
}

bool ModuleInput::GetKeyBindingRepeat(const char * binding_name)
{
	if (!keys_repeat.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}
}

bool ModuleInput::GetKeyBindingUp(const char * binding_name)
{
	if (!keys_up.empty())
	{
		for (vector<KeyBinding>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (TextCmp(binding_name, (*it).binding_name))
				return true;
		}
	}

	return false;
}

void ModuleInput::SetKeyBinding(const char * key, const char * binding_name)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (TextCmp(keyboard[i].binding_name, binding_name))
		{
			keyboard[i].binding_name = "";
		}

		if (i == CharToKey(key))
		{
			keyboard[i].binding_name = binding_name;
		}
	}
}

const char * ModuleInput::GetKeyBinding(const char * binding_name)
{
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (TextCmp(keyboard[i].binding_name, binding_name))
		{
			return KeyToChar(keyboard[i].key);
		}
	}

	return "";
}

bool ModuleInput::GetKeyboardInput(string& input)
{
	if (!text_input.empty())
	{
		input = text_input;
		return true;
	}

	return false;
}

void ModuleInput::ClearKeyboardInput()
{
	text_input.clear();
}

KEY_STATE ModuleInput::GetMouseButton(int id) const
{
	return mouse_buttons[id];
}

int ModuleInput::CharToKey(const char * key)
{
	return SDL_GetScancodeFromKey(SDL_GetKeyFromName(key));
}

const char * ModuleInput::KeyToChar(int key)
{
	return SDL_GetScancodeName((SDL_Scancode)key);
}

void ModuleInput::AddKeyDown(KeyBinding k)
{
	keys_down.push_back(k);
}

void ModuleInput::AddKeyRepeat(KeyBinding k)
{
	keys_repeat.push_back(k);
}

void ModuleInput::AddKeyUp(KeyBinding k)
{
	keys_up.push_back(k);
}

bool ModuleInput::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

int ModuleInput::GetMouseX() const
{
	return mouse_x;
}

int ModuleInput::GetMouseY() const
{
	return mouse_y;
}

int ModuleInput::GetMouseZ() const
{
	return mouse_x;
}

int ModuleInput::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int ModuleInput::GetMouseYMotion() const
{
	return mouse_y_motion;
}
