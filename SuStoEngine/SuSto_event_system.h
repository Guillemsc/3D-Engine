#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

#include <vector>
#define MAX_KEYS 300

class UIElement;

enum UIEventType
{
	MOUSE_CLICK,
	KEYBOARD,

	UIEVENT_NULL
};

//enum KeyEvent
//{
//	KEY_DOWN,
//	KEY_UP,
//	KEY_REPEAT,
//
//	KEY_NULL
//};

struct Key
{
	int key = 0;
	//KeyEvent key_event = KeyEvent::KEY_NULL;

	bool operator == (const Key& comp_key)
	{
		if (key == comp_key.key)
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

		const bool GetKeyDown(int id);
		const bool GetKeyRepeat(int id);
		const bool GetKeyUp(int id);
		const bool GetKeyDown(const char* key);
		const bool GetKeyRepeat(const char* key);
		const bool GetKeyUp(const char* key);

		// Those lists are filled and cleaned every frame
		std::vector<Key> keys_down;
		std::vector<Key> keys_repeat;
		std::vector<Key> keys_up;
	};

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
