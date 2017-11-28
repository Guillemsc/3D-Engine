#ifndef SUSTO_EVENT_SYSTEM
#define SUSTO_EVENT_SYSTEM

class UIElement;

class UIEvent
{
public:
	UIEvent();
	~UIEvent();

	union MouseClick
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
};

#endif
