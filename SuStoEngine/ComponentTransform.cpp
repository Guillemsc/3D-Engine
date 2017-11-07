#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleInput.h"

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
	local_position = float3(0, 0, 0);
	local_rotation_euler = float3(0, 0, 0);
	local_rotation_quat = Quat::identity;
	local_scale = float3(1, 1, 1);

	global_transform.SetIdentity();
}

void ComponentTransform::Update()
{
	float4x4 a = local_transform * local_transform;
}

void ComponentTransform::CleanUp()
{
}

const float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

const void ComponentTransform::SetPosition(const float3 & pos)
{
	local_position = pos;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetRotation(const float3 & rotation)
{
	float3 diff = rotation - local_rotation_euler;
	Quat quat_diff = Quat::FromEulerXYZ(diff.x*DEGTORAD, diff.y*DEGTORAD, diff.z*DEGTORAD);
	local_rotation_quat = local_rotation_quat * quat_diff;
	local_rotation_euler = rotation;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetRotation(const Quat & quat)
{
	local_rotation_quat = quat;
	local_rotation_euler = local_rotation_quat.ToEulerXYZ() * RADTODEG;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetScale(const float3 & scale)
{
	local_scale = scale;

	RecalculateLocalTransform();
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
	return local_position;
}

const float3 ComponentTransform::GetGlobalPosition() const
{
	float3 position = float3::zero;
	float4x4 dummy = float4x4::identity;
	float3 dummy2 = float3::zero;

	global_transform.Decompose(position, dummy, dummy2);

	return position;
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return local_rotation_euler;
}

const Quat ComponentTransform::GetRotationQuat() const
{
	return local_rotation_quat;
}

const float3 ComponentTransform::GetScale() const
{
	return local_scale;
}

void ComponentTransform::RecalculateLocalTransform()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation_quat, local_scale);
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

void ComponentTransform::InspectorDraw(std::vector<Component*> components)
{
	float3 position = GetPosition();
	float3 rotation = GetRotationEuler();
	float3 scale = GetScale();
	 
		if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
			SetPosition(position);

		if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f))
			SetRotation(rotation);

		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f))
			SetScale(scale);
}

