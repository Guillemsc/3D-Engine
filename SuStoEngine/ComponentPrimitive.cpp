#include "ComponentPrimitive.h"

ComponentPrimitive::ComponentPrimitive(ComponentType type, GameObject * owner) : Component(type, owner)
{

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
