#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleInput.h"
#include "JSONLoader.h"

ComponentTransform::ComponentTransform(GameObject * owner, std::string unique_id) : Component(ComponentType::TRANSFORM, owner, unique_id, true)
{
	CONSOLE_LOG("Component Transform Created");
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
	if (local_position.x != pos.x || local_position.y != pos.y || local_position.z != pos.z)
	{
		local_position = pos;

		RecalculateLocalTransform();
	}
}

const void ComponentTransform::Translate(const float3 & pos)
{
	if (pos.x != 0 || pos.y != 0 || pos.z != 0)
	{
		local_position += pos;

		RecalculateLocalTransform();
	}
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

const void ComponentTransform::Rotate(const float3 & rotate)
{
	SetRotation(GetRotationEuler() + rotate);

	RecalculateLocalTransform();
}

const void ComponentTransform::SetScale(const float3 & scale)
{
	local_scale = scale;

	RecalculateLocalTransform();
}

const void ComponentTransform::Scale(const float3 & scale)
{
	local_scale += scale;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetLocalTransform(const float4x4 & transform)
{
	local_transform = transform;

	transform.Decompose(local_position, local_rotation_quat, local_scale);
	local_scale = local_rotation_quat.ToEulerXYZ();
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

const void ComponentTransform::SetGlobalTransform(const float4x4 & transform)
{
	if (GetOwner()->GetParent() != nullptr)
	{
		float4x4 new_local = transform * GetOwner()->GetParent()->transform->GetGlobalTransform().Inverted();
		SetLocalTransform(new_local);
	}
	else
		SetLocalTransform(transform);
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

void ComponentTransform::OnLoadAbstraction(DataAbstraction& abs)
{
	float3 position = abs.GetFloat3("position");
	float4 rotation = abs.GetFloat4("rotation");
	float3 scale = abs.GetFloat3("scale");

	SetPosition(position);
	SetRotation(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	SetScale(scale);
}

void ComponentTransform::OnSaveAbstraction(DataAbstraction& abs)
{
	abs.AddFloat3("position", local_position);
	abs.AddFloat4("rotation", float4(local_rotation_quat.x, local_rotation_quat.y, local_rotation_quat.z, local_rotation_quat.w));
	abs.AddFloat3("scale", local_scale);
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

