#ifndef __MODULE_EVENT_SYSTEM_H__
#define __MODULE_EVENT_SYSTEM_H__

#include "Module.h"
#include "GameObject.h"
#include <functional>

enum EventType
{
	ET_NULL,
	ET_GAMEOBJECT_CREATE,
	ET_GAMEOBJECT_DESTROY,
	ET_GAMEOBJECT_CHANGE_PARENT,

	ET_COMPONENT_CREATE,
	ET_COMPONENT_DESTROY,
	ET_COMPONENT_ACTIVATE,
	ET_COMPONENT_DEACTIVATE,
};

struct EventSuscribers
{
	EventType type = ET_NULL;
	std::vector<std::function<void(Event)>> sucribed_functions;
};

class Event
{
public:
	Event(const EventType& type);
	EventType GetType() const;

	// GameObjects
	struct GameObjectCreate
	{
		GameObject* go = nullptr;
	}
	game_object_create;

	struct GameObjectDestroy
	{
		GameObject* go = nullptr;
	} 
	game_object_destroy;

	struct GameObjectChangeParent
	{
		GameObject* go = nullptr;
		GameObject* old_parent = nullptr;
		GameObject* new_parent = nullptr;
	}
	game_object_change_parent;

	// Components
	struct ComponentCreate
	{
		Component* component = nullptr;
	}
	component_create;

	struct ComponentDestroy
	{
		Component* component = nullptr;
	}
	component_destroy;

	struct ComponentActivate
	{
		Component* component = nullptr;
	}
	component_activate;

	struct ComponentDeactivate
	{
		Component* component = nullptr;
	}
	component_deactivate;

private:
	EventType type = ET_NULL;
};

class ModuleEventSystem : public Module
{
public:
	ModuleEventSystem(bool start_enabled = true);
	~ModuleEventSystem();

	bool Awake();
	bool Start();
	bool CleanUp();

	void Suscribe(EventType type, std::function<void(Event)> function);
	void UnSuscribe(std::function<void(Event)> function);
	void Send(Event ev);

private:
	std::vector<EventSuscribers> suscribers;
};


#endif // !__MODULE_EVENT_SYSTEM_H__
