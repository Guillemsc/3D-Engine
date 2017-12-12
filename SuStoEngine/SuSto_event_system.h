#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

#include <vector>

#define MAX_KEYS 300
#define MAX_MOUSE 5

class UIElement;

enum UIEventType
{
	MOUSE_CLICK,
	KEYBOARD,

	UIEVENT_NULL
};

enum UIKeyEvent
{
	UI_KEY_IDLE,
	UI_KEY_DOWN,
	UI_KEY_UP,
	UI_KEY_REPEAT
};

struct Key
{
	int key = 0;
	UIKeyEvent key_event = UIKeyEvent::UI_KEY_IDLE;

	bool operator == (const Key& comp_key)
	{
		if (key == comp_key.key)
			return true;
		return false;
	}

	bool operator == (int comp_key_event)
	{
		if (key_event == comp_key_event)
			return true;
		return false;
	}
};

class UIEvent
{
public:
	UIEvent(UIEventType type);
	~UIEvent();

	struct MouseClick
	{
		bool left_button = false;
		bool right_button = false;
		bool wheel_button = false;
	};

	struct Keyboard
	{
		Key* keyboard = nullptr;
		UIKeyEvent mouse_buttons[MAX_MOUSE];

		const bool GetKeyDown(int id);
		const bool GetKeyRepeat(int id);
		const bool GetKeyUp(int id);

		// Those lists are filled and cleaned every frame
		std::vector<Key> keys_down;
		std::vector<Key> keys_repeat;
		std::vector<Key> keys_up;
	};

public:
	UIKeyEvent GetMouseButton(int id) const;

private:
	MouseClick	mouse_click;
	Keyboard	keyboard;

	UIEventType event_type = UIEventType::UIEVENT_NULL;
};

class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	void SendEvent(UIEvent ev);
};

#endif
