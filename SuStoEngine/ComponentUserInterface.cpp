#include "ComponentUserInterface.h"

ComponentUserInterface::ComponentUserInterface(GameObject * owner, std::string unique_id) : Component(UI, owner, unique_id)
{
	LOG_OUTPUT("Component User Interface Created");
	SetName("User Interface");
}

ComponentUserInterface::~ComponentUserInterface()
{
}

void ComponentUserInterface::Start()
{
}

void ComponentUserInterface::Update()
{
}

void ComponentUserInterface::CleanUp()
{
}

void ComponentUserInterface::InspectorDraw(std::vector<Component*> components)
{
}

void ComponentUserInterface::OnEnable()
{
}

void ComponentUserInterface::OnDisable()
{
}

void ComponentUserInterface::OnLoadSerialize(JSON_Doc config)
{
}

void ComponentUserInterface::OnSaveSerialize(JSON_Doc config)
{
}
