#include "ComponentTransform.h"
#include "imgui.h"

ComponentTransfrom::ComponentTransfrom(GameObject * owner) : Component(ComponentType::TRANSFORM, owner)
{
	LOG_OUTPUT("Component Transform Created");
	SetName("Transform");
}

ComponentTransfrom::~ComponentTransfrom()
{
}

float3 ComponentTransfrom::GetPosition()
{
	return float3();
}

float3 ComponentTransfrom::GetRotation()
{
	return float3();
}

float3 ComponentTransfrom::GetScale()
{
	return float3();
}

void ComponentTransfrom::InspectorDraw()
{
	ImGui::Text(GetName());
}

void ComponentTransfrom::OnCreate()
{
}

void ComponentTransfrom::OnDestroy()
{
}

void ComponentTransfrom::OnEnable()
{
}

void ComponentTransfrom::OnDisable()
{
}
