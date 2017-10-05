#include "Component.h"

Component::Component(ComponentType _type, GameObject* _owner)
{
	type = _type;
	owner = _owner;

	enabled = true;

	OnCreate();
}

Component::~Component()
{
	Destroy();
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
		enabled = true;
		OnDisable();
	}
}

bool Component::GetEnabled()
{
	return enabled;
}

void Component::SetEnabled(bool set)
{
	set ? Enable() : Disable();
}

void Component::Destroy()
{
	OnDestroy();
}

GameObject * Component::GetOwner()
{
	return owner;
}

ComponentType Component::GetType()
{
	return type;
}
