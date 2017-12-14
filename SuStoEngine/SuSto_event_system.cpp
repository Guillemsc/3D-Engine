#include "SuSto_event_system.h"
#include "SuStoUI.h"


UIEvent::UIEvent()
{

}

UIEvent::~UIEvent()
{
}

std::vector<UIKey> UIEventSystem::GetKeysDown() const
{
	return keys_down;
}

std::vector<UIKey> UIEventSystem::GetKeysRepeat() const
{
	return keys_repeat;
}

std::vector<UIKey> UIEventSystem::GetKeysUp() const
{
	return keys_up;
}

const bool UIEventSystem::GetKeyDown(const char * key)
{
	return GetKeyDown(CharToKey(key));
}

const bool UIEventSystem::GetKeyRepeat(const char * key)
{
	return GetKeyRepeat(CharToKey(key));
}

const bool UIEventSystem::GetKeyUp(const char * key)
{
	return  GetKeyUp(CharToKey(key));
}

void UIEventSystem::AddKeyDown(const UIKey & k)
{
	keys_down.push_back(k);
}

void UIEventSystem::AddKeyRepeat(const UIKey & k)
{
	keys_repeat.push_back(k);
}

void UIEventSystem::AddKeyUp(const UIKey & k)
{
	keys_up.push_back(k);
}

int UIEventSystem::GetMouseX() const
{
	return mouse_x;
}

int UIEventSystem::GetMouseY() const
{
	return mouse_y;
}

int UIEventSystem::GetMouseWheel() const
{
	return mouse_wheel;
}

int UIEventSystem::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int UIEventSystem::GetMouseYMotion() const
{
	return mouse_y_motion;
}

void UIEventSystem::SetMouseX(const int & x)
{
	mouse_x = x;
}

void UIEventSystem::SetMouseY(const int & y)
{
	mouse_y = y;
}

void UIEventSystem::SetMouseWheel(const int & wheel)
{
	mouse_wheel = wheel;
}

void UIEventSystem::SetMouseXMotion(const int & x_motion)
{
	mouse_x_motion = x_motion;
}

void UIEventSystem::SetMouseYMotion(const int & y_motion)
{
	mouse_y_motion = y_motion;
}

UIEventSystem::UIEventSystem()
{
	keyboard = new UIKey[MAX_KEYS];

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i].key = i;
}

UIEventSystem::~UIEventSystem()
{
}

void UIEventSystem::CleanUp()
{
	delete[] keyboard;
}

void UIEventSystem::SendEvent(UIEvent ev)
{
}

void UIEventSystem::ResetInput()
{
	text_input.clear();
	keys_down.clear();
	keys_repeat.clear();
	keys_up.clear();
}

const bool UIEventSystem::GetKeyDown(int id)
{
	if (!keys_down.empty())
	{
		for (std::vector<UIKey>::iterator it = keys_down.begin(); it != keys_down.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool UIEventSystem::GetKeyRepeat(int id)
{
	if (!keys_repeat.empty())
	{
		for (std::vector<UIKey>::iterator it = keys_repeat.begin(); it != keys_repeat.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}

const bool UIEventSystem::GetKeyUp(int id)
{
	if (!keys_up.empty())
	{
		for (std::vector<UIKey>::iterator it = keys_up.begin(); it != keys_up.end(); it++)
		{
			if (id == (*it).key)
				return true;
		}
	}

	return false;
}
