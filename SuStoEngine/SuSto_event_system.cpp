#include "SuSto_event_system.h"
#include "SuStoUI.h"

//UIEvent::UIEvent(UIEventType type)
//{
//	event_type = type;
//
//	keyboard.keyboard = new Key[MAX_KEYS];
//
//	for (int i = 0; i < MAX_KEYS; ++i)
//		keyboard.keyboard[i].key = i;
//}

UIEvent::UIEvent()
{
	keyboard = new Key[MAX_KEYS];

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i].key = i;
}

UIEvent::~UIEvent()
{
}

std::vector<Key> UIEvent::GetKeysDown() const
{
	return keys_down;
}

std::vector<Key> UIEvent::GetKeysRepeat() const
{
	return keys_repeat;
}

std::vector<Key> UIEvent::GetKeysUp() const
{
	return keys_up;
}

void UIEvent::AddKeyDown(const Key & k)
{
	keys_down.push_back(k);
}

void UIEvent::AddKeyRepeat(const Key & k)
{
	keys_repeat.push_back(k);
}

void UIEvent::AddKeyUp(const Key & k)
{
	keys_up.push_back(k);
}

int UIEvent::GetMouseX() const
{
	return mouse_x;
}

int UIEvent::GetMouseY() const
{
	return mouse_y;
}

int UIEvent::GetMouseWheel() const
{
	return mouse_wheel;
}

int UIEvent::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int UIEvent::GetMouseYMotion() const
{
	return mouse_y_motion;
}

void UIEvent::SetMouseX(const int & x)
{
	mouse_x = x;
}

void UIEvent::SetMouseY(const int & y)
{
	mouse_y = y;
}

void UIEvent::SetMouseWheel(const int & wheel)
{
	mouse_wheel = wheel;
}

void UIEvent::SetMouseXMotion(const int & x_motion)
{
	mouse_x_motion = x_motion;
}

void UIEvent::SetMouseYMotion(const int & y_motion)
{
	mouse_y_motion = y_motion;
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