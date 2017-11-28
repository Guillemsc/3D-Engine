#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

class UIElement;

class UIEvent
{
public:
	UIEvent();
	~UIEvent();

	struct MouseClick
	{
		bool left_button = false;
		bool right_button = false;
		bool wheel_button = false;
	};

};

class EventSystem
{
public:
	EventSystem();
	~EventSystem();

	void SendEvend(UIEvent ev);
};

#endif
