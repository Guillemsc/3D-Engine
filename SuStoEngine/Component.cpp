#include "Component.h"

Component::Component(ComponentType _type, GameObject* _owner, std::string _unique_id)
{
	type = _type;
	owner = _owner;
	unique_id = _unique_id;

	enabled = true;
}

Component::~Component()
{

}

void Component::Enable()
{
	if (!enabled)
	{
		enabled = true;
		OnEnable();
	}
}

void Component::Disable()
{
	if (enabled)
	{
		enabled = false;
		OnDisable();
	}
}

const bool Component::GetEnabled() const
{
	return enabled;
}

void Component::SetEnabled(const bool& set)
{
	set ? Enable() : Disable();
}

void Component::SetName(const char * new_name)
{
	name = new_name;
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
