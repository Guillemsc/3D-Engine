#include "ComponentPrimitive.h"

ComponentPrimitive::ComponentPrimitive(GameObject * owner) : Component(PRIMITIVE, owner)
{
	LOG_OUTPUT("Component Primitive Created");
}

ComponentPrimitive::~ComponentPrimitive()
{
}

void ComponentPrimitive::Update()
{
}

void ComponentPrimitive::Cube()
{
	type = CUBE;


}

void ComponentPrimitive::OnCreate()
{
	Cube();
}

void ComponentPrimitive::OnDestroy()
{
}

void ComponentPrimitive::OnEnable()
{
}

void ComponentPrimitive::OnDisable()
{
}
