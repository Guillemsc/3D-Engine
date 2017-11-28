#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

class UIElement;

enum UIEventType
{
	MOUSE_CLICK,

	UIEVENT_NULL
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

	MouseClick mouse_click;

	UIEventType event_type = UIEventType::UIEVENT_NULL;
};

class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	void SendEvend(UIEvent ev);
};

#endif
