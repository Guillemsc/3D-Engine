#include "Component.h"
#include "App.h"
#include "ModuleEventSystem.h"

Component::Component(ComponentType _type, GameObject* _owner, std::string _unique_id)
{
	type = _type;
	owner = _owner;
	unique_id = _unique_id;
	name = "---------- NO NAME ----------";

	SetActive(true);
	SetCanDestroy(true);
	SetOnePerGo(false);
	SetCanDisable(true);
}

Component::~Component()
{

}

void Component::SetActive(const bool & set)
{
	if (active != set)
	{
		active = set;

		if(active)
		{
			Event ev(EventType::ET_COMPONENT_ACTIVATE);
			ev.component_activate.component = this;
			App->event_system->Send(ev);
		}
		else
		{
			Event ev(EventType::ET_COMPONENT_DEACTIVATE);
			ev.component_deactivate.component = this;
			App->event_system->Send(ev);
		}
	}
}

bool Component::GetActive() const
{
	return active;
}

void Component::SetName(const char * new_name)
{
	name = new_name;
}

void Component::SetOnePerGo(bool set)
{
	one_per_go = set;
}

void Component::SetCanDestroy(bool set)
{
	can_destroy = set;
}

void Component::SetCanDisable(bool set)
{
	can_disable = set;
}

bool Component::GetOnePerGo() const
{
	return one_per_go;
}

bool Component::GetCanDestroy() const
{
	return can_destroy;
}

bool Component::GetCanDisable() const
{
	return can_disable;
}

void Component::ForceUid(std::string uid)
{
	unique_id = uid;
}

GameObject * Component::GetOwner() const
{
	return owner;
}

const ComponentType Component::GetType() const
{
	return type;
}

const char* Component::GetName() const
{
	return name;
}

std::string Component::GetUniqueId()
{
	return unique_id;
}
