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
	local_transform.SetIdentity();
	global_transform.SetIdentity();
}

void ComponentTransform::Update()
{
	float4x4 a = local_transform * local_transform;
}

void ComponentTransform::CleanUp()
{
}

const void ComponentTransform::SetPosition(float3 & pos)
{
	local_transform[3][0] = pos.x;
	local_transform[3][1] = pos.y;
	local_transform[3][2] = pos.z;
}

const void ComponentTransform::SetRotation(const Quat & quater)
{
	local_transform.FromQuat(quater);
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

const float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

const void ComponentTransform::SetLocalTransform(const float4x4 & transform)
{
	local_transform = transform;
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

const float3 ComponentTransform::GetPosition() const
{
	return float3(local_transform[3][0], local_transform[3][1], local_transform[3][2]);
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return local_transform.ToEulerXYZ();
}

const float3 ComponentTransform::GetScale() const
{
	return local_transform.GetScale();
}

void ComponentTransform::InspectorDraw(std::vector<Component*> components)
{
	float3 position = GetPosition();
	float3 rotation = GetRotationEuler();
	float3 scale = GetScale();

	if (ImGui::InputFloat3("Position", (float*)&position))
		SetPosition(position);

	if (ImGui::InputFloat3("Rotation", (float*)&rotation))
		SetRotation(Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z));

	if (ImGui::InputFloat3("Scale", (float*)&scale)) 
	{

	}

}

