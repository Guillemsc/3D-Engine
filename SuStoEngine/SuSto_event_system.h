#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

#include <vector>
#include <string>
#include <functional>

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

enum UIMouseClick
{
	UI_NULL_CLICK,		// 0
	UI_LEFT_CLICK,		// 1
	UI_MID_CLICK,		// 2
	UI_RIGHT_CLICK		// 3
};

struct UIKey
{
public:
	int key = 0;
	UIKeyEvent state = UIKeyEvent::UI_KEY_IDLE;

	bool operator == (const UIKey& comp_key)
	{
		if (key == comp_key.key)
			return true;
		return false;
	}

	bool operator == (UIKeyEvent ev)
	{
		if (state == ev)
			return true;
		return false;
	}
};

class UIEvent
{
public:
	UIEvent();
	~UIEvent();
};

class UIEventSystem
{
public:
	UIEventSystem();
	~UIEventSystem();
	void CleanUp();

	const bool GetKeyDown(const char* key);
	const bool GetKeyRepeat(const char* key);
	const bool GetKeyUp(const char* key);

	void AddKeyDown(const UIKey& k);
	void AddKeyRepeat(const UIKey& k);
	void AddKeyUp(const UIKey& k);

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

	void SendEvent(UIEvent ev);

	void ResetInput();

private:
	const bool GetKeyDown(int id);
	const bool GetKeyRepeat(int id);
	const bool GetKeyUp(int id);

	std::vector<UIKey> GetKeysDown() const;
	std::vector<UIKey> GetKeysRepeat() const;
	std::vector<UIKey> GetKeysUp() const;

public:
	UIKey*		keyboard = nullptr;
	UIKeyEvent	mouse_buttons[MAX_MOUSE];

	std::string text_input;

	bool		right_clicking = false;

	std::function<int(const char*)> CharToKey;
	std::function<const char*(int)> KeyToChar;

private:
	int		    mouse_x = 0;
	int		    mouse_y = 0;
	int		    mouse_wheel = 0;
	int		    mouse_x_motion = 0;
	int		    mouse_y_motion = 0;

private:
	std::vector<UIKey> keys_down;
	std::vector<UIKey> keys_repeat;
	std::vector<UIKey> keys_up;
};

#endif
