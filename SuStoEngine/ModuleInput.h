#pragma once
#include "Module.h"
#include "Globals.h"
#include "Functions.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
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
	const char* binding_name = nullptr;
	int         key;
	KEY_STATE   state = KEY_IDLE;

	bool operator == (KeyBinding bin)
	{
		if (key == bin.key)
			return true;
		return false;
	}
};

class ModuleInput : public Module
{
public:
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Awake();
	bool PreUpdate();
	bool CleanUp();

	bool GetKeyDown(int id);
	bool GetKeyRepeat(int id);
	bool GetKeyUp(int id);
	bool GetKeyDown(const char* key);
	bool GetKeyRepeat(const char* key);
	bool GetKeyUp(const char* key);

	bool GetKeyBindingDown(const char* binding_name);
	bool GetKeyBindingRepeat(const char* binding_name);
	bool GetKeyBindingUp(const char* binding_name);
	void SetKeyBinding(const char* key, const char* binding_name);

	KEY_STATE GetMouseButton(int id) const;

	bool GetWindowEvent(EventWindow ev);

	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseZ() const;
	int GetMouseXMotion() const;
	int GetMouseYMotion() const;

private:
	int CharToKey(const char* key);
	void AddKeyDown(KeyBinding k);
	void AddKeyRepeat(KeyBinding k);
	void AddKeyUp(KeyBinding k);

private:
	vector<KeyBinding> keys_down;
	vector<KeyBinding> keys_repeat;
	vector<KeyBinding> keys_up;
	bool			   windowEvents[WE_COUNT];
	KeyBinding*		   keyboard;
	KEY_STATE		   mouse_buttons[MAX_MOUSE_BUTTONS];
	int				   mouse_x = 0;
	int				   mouse_y = 0;
	int				   mouse_z = 0;
	int				   mouse_x_motion = 0;
	int				   mouse_y_motion = 0;
};