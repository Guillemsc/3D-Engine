#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

#include <vector>
#include <string>

#define MAX_KEYS 300
#define MAX_MOUSE 5

class UIElement;

//enum UIEventType
//{
//	MOUSE_CLICK,
//	KEYBOARD,
//
//	UIEVENT_NULL
//};

enum UIKeyEvent
{
	UI_KEY_IDLE,
	UI_KEY_DOWN,
	UI_KEY_UP,
	UI_KEY_REPEAT
};

enum MouseClick
{
	UI_NULL_CLICK,		// 0
	UI_LEFT_CLICK,		// 1
	UI_MID_CLICK,		// 2
	UI_RIGHT_CLICK		// 3
};

struct Key
{
public:
	int key = 0;
	UIKeyEvent state = UIKeyEvent::UI_KEY_IDLE;

	bool operator == (const Key& comp_key)
	{
		if (key == comp_key.key)
			return true;
		return false;
	}

	bool operator == (int comp_key_event)
	{
		if (state == comp_key_event)
			return true;
		return false;
	}
};

class UIEvent
{
public:
	UIEvent();
	~UIEvent();

	std::vector<Key> GetKeysDown() const;
	std::vector<Key> GetKeysRepeat() const;
	std::vector<Key> GetKeysUp() const;

	void AddKeyDown(const Key& k);
	void AddKeyRepeat(const Key& k);
	void AddKeyUp(const Key& k);

	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseWheel() const;
	int GetMouseXMotion() const;
	int GetMouseYMotion() const;

	void SetMouseX(const int& x);
	void SetMouseY(const int& y);
	void SetMouseWheel(const int& wheel);
	void SetMouseXMotion(const int& x_motion);
	void SetMouseYMotion(const int& y_motion);


private:
	int				   mouse_x = 0;
	int				   mouse_y = 0;
	int				   mouse_wheel = 0;
	int				   mouse_x_motion = 0;
	int				   mouse_y_motion = 0;

public:
	Key*			   keyboard = nullptr;
	UIKeyEvent		   mouse_buttons[MAX_MOUSE];

	std::string		   text_input;

	bool			   right_clicking = false;

private:
	std::vector<Key> keys_down;
	std::vector<Key> keys_repeat;
	std::vector<Key> keys_up;
};

class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	void SendEvent(UIEvent ev);
};

#endif
