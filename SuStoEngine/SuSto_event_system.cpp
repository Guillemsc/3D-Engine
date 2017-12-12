#include "SuSto_event_system.h"
#include "SuStoUI.h"

UIEvent::UIEvent(UIEventType type)
{
	event_type = type;

	keyboard.keyboard = new Key[MAX_KEYS];

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard.keyboard[i].key = i;
}

UIEvent::~UIEvent()
{
}

UIKeyEvent UIEvent::GetMouseButton(int id) const
{
	return keyboard.mouse_buttons[id];
}

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::SendEvent(UIEvent ev)
{
}

const bool UIEvent::Keyboard::GetKeyDown(int id)
{
	if (!keys_down.empty())
	{
		for (std::vector<Key>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool UIEvent::Keyboard::GetKeyRepeat(int id)
{
	if (!keys_repeat.empty())
	{
		for (std::vector<Key>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool UIEvent::Keyboard::GetKeyUp(int id)
{
	if (!keys_up.empty())
	{
		for (std::vector<Key>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

/*const bool UIEvent::Keyboard::GetKeyDown(const char * key)
{
	return GetKeyDown(CharToKey(key));
}

const bool UIEvent::Keyboard::GetKeyRepeat(const char * key)
{
	return GetKeyRepeat(CharToKey(key));
}

const bool UIEvent::Keyboard::GetKeyUp(const char * key)
{
	return GetKeyUp(CharToKey(key));
}*/
