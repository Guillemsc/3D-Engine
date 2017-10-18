#include "ComponentTransform.h"
#include "imgui.h"

ComponentTransform::ComponentTransform(GameObject * owner) : Component(ComponentType::TRANSFORM, owner)
{
	LOG_OUTPUT("Component Transform Created");
	SetName("Transform");
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Start()
{
	transform.SetIdentity();
}

void ComponentTransform::Update()
{
}

void ComponentTransform::CleanUp()
{
}

const void ComponentTransform::SetPosition(float3 & pos)
{
	transform[3][0] = pos.x;
	transform[3][1] = pos.y;
	transform[3][2] = pos.z;
}

const void ComponentTransform::SetRotationQuat(Quat & quater)
{
	transform.FromQuat(quater);
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

const float4x4 ComponentTransform::GetTransform() const
{
	return transform;
}

const float3 ComponentTransform::GetPosition() const
{
	return float3(transform[3][0], transform[3][1], transform[3][2]);
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return transform.ToEulerXYZ();
}

const float3 ComponentTransform::GetScale() const
{
	float x = Sqrt(Pow(transform[0][0], 2) + Pow(transform[0][1], 2) + Pow(transform[0][2], 2));
	float y = Sqrt(Pow(transform[1][0], 2) + Pow(transform[1][1], 2) + Pow(transform[1][2], 2));
	float z = Sqrt(Pow(transform[2][0], 2) + Pow(transform[2][1], 2) + Pow(transform[2][2], 2));

	return float3(x, y, z);
}

void ComponentTransform::InspectorDraw(std::vector<Component*> components)
{
	float3 position = GetPosition();
	float3 rotation = GetRotationEuler();
	float3 scale = GetScale();

	if (ImGui::InputFloat3("Position", (float*)&position))
		SetPosition(position);
	if (ImGui::InputFloat3("Rotation", (float*)&rotation))
		SetRotationQuat(Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z));
	if (ImGui::InputFloat3("Scale", (float*)&scale)) {

	}

}

