#include "ModuleEventSystem.h"

ModuleEventSystem::ModuleEventSystem(bool start_enabled) : Module(start_enabled)
{
}

ModuleEventSystem::~ModuleEventSystem()
{
}

bool ModuleEventSystem::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleEventSystem::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleEventSystem::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleEventSystem::Suscribe(EventType type, std::function<void(Event)> function)
{
	if (type != ET_NULL)
	{
		bool found = false;
		for (std::vector<EventSuscribers>::iterator it = suscribers.begin(); it != suscribers.end(); ++it)
		{
			if ((*it).type == type)
			{
				for (std::vector<std::function<void(Event)>>::iterator sus = (*it).sucribed_functions.begin(); sus != (*it).sucribed_functions.end(); ++sus)
				{
					if ((*sus).target_type() == function.target_type())
					{
						return;
					}
				}

				(*it).sucribed_functions.push_back(function);
			}
		}

		if (!found)
		{
			EventSuscribers es;
			es.type = type;
			es.sucribed_functions.push_back(function);
			suscribers.push_back(es);
		}
	}
}

void ModuleEventSystem::UnSuscribe(std::function<void(Event)> function)
{
	for (std::vector<EventSuscribers>::iterator it = suscribers.begin(); it != suscribers.end(); ++it)
	{
		for (std::vector<std::function<void(Event)>>::iterator sus = (*it).sucribed_functions.begin(); sus != (*it).sucribed_functions.end(); ++sus)
		{
			if ((*sus).target_type() == function.target_type())
			{
				(*it).sucribed_functions.erase(sus);
				break;
			}
		}
	}
}

void ModuleEventSystem::Send(Event ev)
{
	if (ev.GetType() != ET_NULL)
	{
		for (std::vector<EventSuscribers>::iterator it = suscribers.begin(); it != suscribers.end(); ++it)
		{
			if (ev.GetType() == (*it).type)
			{
				for (std::vector<std::function<void(Event)>>::iterator sus = (*it).sucribed_functions.begin(); sus != (*it).sucribed_functions.end(); ++sus)
				{
					if ((*sus))
					{
						(*sus)(ev);
					}
				}
			}
		}
	}
}

Event::Event(const EventType& _type)
{
	type = _type;
}

EventType Event::GetType() const
{
	return type;
}
