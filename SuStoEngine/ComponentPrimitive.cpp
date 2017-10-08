#include "ComponentPrimitive.h"
#include "imgui.h"

ComponentPrimitive::ComponentPrimitive(GameObject * owner) : Component(PRIMITIVE, owner)
{
	LOG_OUTPUT("Component Primitive Created");
	SetName("Primitive");
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

void ComponentPrimitive::InspectorDraw()
{
	ImGui::Text(GetName());
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
