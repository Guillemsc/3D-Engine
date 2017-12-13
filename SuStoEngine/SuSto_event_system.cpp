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

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::SendEvent(UIEvent ev)
{
}