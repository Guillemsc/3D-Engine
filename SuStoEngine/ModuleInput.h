#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

struct KeyBinding
{
	char* binding_name;
	char* key;
};

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Awake();
	bool PreUpdate();
	bool CleanUp();

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetKey(const char* key)
	{
		int id = CharToKey(key);
		
		return GetKey(id);
	}

	KEY_STATE GetKeyBinding(const char* binding)
	{

	}

	void SetKeyBinding(const char* key, const char* binding_name)
	{

	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	bool GetWindowEvent(EventWindow ev);

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

private:
	int CharToKey(const char* key);

private:
	vector<KeyBinding> key_bindings;
	bool			   windowEvents[WE_COUNT];
	KEY_STATE*		   keyboard;
	KEY_STATE		   mouse_buttons[MAX_MOUSE_BUTTONS];
	int				   mouse_x = 0;
	int				   mouse_y = 0;
	int				   mouse_z = 0;
	int				   mouse_x_motion = 0;
	int				   mouse_y_motion = 0;
	//int mouse_z_motion;
};