#include "ComponentTransform.h"
#include "GameObject.h"
#include "imgui.h"
#include "App.h"
#include "ModuleInput.h"
#include "JSONLoader.h"

ComponentTransform::ComponentTransform(GameObject * owner, std::string unique_id) : Component(ComponentType::TRANSFORM, owner, unique_id)
{
	CONSOLE_LOG("Component Transform Created");
	SetName("Transform");

	SetCanDestroy(false);
	SetOnePerGo(true);
	SetCanDisable(false);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Start()
{
	global_transform.SetIdentity();
}

void ComponentTransform::Update()
{
	
}

void ComponentTransform::CleanUp()
{
}

const float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

const void ComponentTransform::SetLocalPosition(const float3 & pos)
{
	local_position = float4x4::FromTRS(pos, Quat::identity, float3::one);

	RecalculateLocalTransform();
}

const void ComponentTransform::SetLocalRotation(const float3 & rotation)
{
	local_rotation_quat = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);

	local_rotation_euler = rotation;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetLocalRotation(const Quat & quat)
{
	local_rotation_quat = quat;
	local_rotation_euler = local_rotation_quat.ToEulerXYZ() * RADTODEG;

	RecalculateLocalTransform();
}

const void ComponentTransform::SetScale(const float3 & scale)
{
	float3 scal_mod = scale;

	if (scal_mod.x < 0)
		scal_mod.x = 0.001f;

	if (scal_mod.y < 0)
		scal_mod.y = 0.001f;

	if (scal_mod.z < 0)
		scal_mod.z = 0.001f;

	local_scale = float4x4::FromTRS(float3::zero, Quat::identity, scal_mod);

	RecalculateLocalTransform();
}

const void ComponentTransform::SetLocalPosRotScal(const float3 & pos, const Quat & quater, const float3 & scale)
{
	local_position = float4x4::FromTRS(pos, Quat::identity, float3::one);

	local_rotation_quat = quater;
	local_rotation_euler = local_rotation_quat.ToEulerXYZ() * RADTODEG;

	float3 scal_mod = scale;

	if (scal_mod.x < 0)
		scal_mod.x = 0.001f;

	if (scal_mod.y < 0)
		scal_mod.y = 0.001f;

	if (scal_mod.z < 0)
		scal_mod.z = 0.001f;

	local_scale = float4x4::FromTRS(float3::zero, Quat::identity, scal_mod);

	RecalculateLocalTransform();
}

const void ComponentTransform::Translate(const float3 & pos)
{
	//if (pos.x != 0 || pos.y != 0 || pos.z != 0)
	//{
	//	local_position += pos;

	//	RecalculateLocalTransform();
	//}
}

const void ComponentTransform::Rotate(const float3 & rotate)
{
	/*SetRotation(GetRotationEuler() + rotate);

	RecalculateLocalTransform();*/
}

const void ComponentTransform::Scale(const float3 & scale)
{
	//local_scale += scale;

	//RecalculateLocalTransform();
}

const void ComponentTransform::SetLocalTransform(const float4x4 & transform)
{
	local_transform = transform;

	float3 pos; Quat rot; float3 scal;
	transform.Decompose(pos, rot, scal);

	SetLocalPosRotScal(pos, rot, scal);
}

const float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

const void ComponentTransform::SetGlobalTransform(const float4x4 & transform)
{
	global_transform = transform;

	if (GetOwner()->GetParent() != nullptr)
	{
		float4x4 new_local = GetOwner()->GetParent()->transform->GetGlobalTransform().Inverted() * global_transform;

		SetLocalTransform(new_local);
	}
}

const float3 ComponentTransform::GetLocalPosition() const
{
	float3 ret = float3::zero;
	Quat rot;
	float3 scal;
	local_position.Decompose(ret, rot, scal);
	return ret;
}

const float3 ComponentTransform::GetGlobalPosition() const
{
	float3 position = float3::zero;
	float4x4 dummy = float4x4::identity;
	float3 dummy2 = float3::zero;

	global_transform.Decompose(position, dummy, dummy2);

	return position;
}

const float3 ComponentTransform::GetLocalRotationEuler() const
{
	return local_rotation_euler;
}

const Quat ComponentTransform::GetLocalRotationQuat() const
{
	float3 ret = float3::zero;
	Quat rot;
	float3 scal;
	local_rotation_quat.Decompose(ret, rot, scal);
	return rot;
}

const float3 ComponentTransform::GetScale() const
{
	float3 ret = float3::zero;
	Quat rot;
	float3 scal;
	local_scale.Decompose(ret, rot, scal);
	return scal;
}

void ComponentTransform::RecalculateLocalTransform()
{
	local_transform = local_position * local_rotation_quat * local_scale;
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

void ComponentTransform::OnSaveAbstraction(DataAbstraction & abs)
{
	Quat local_rot = GetLocalRotationQuat();
	abs.AddFloat3("position", GetLocalPosition());
	abs.AddFloat4("rotation", float4(local_rot.x, local_rot.y, local_rot.z, local_rot.w));
	abs.AddFloat3("scale", GetScale());
}

void ComponentTransform::OnLoadAbstraction(DataAbstraction & abs)
{
	float3 position = abs.GetFloat3("position");
	float4 rotation = abs.GetFloat4("rotation");
	float3 scale = abs.GetFloat3("scale");

	SetLocalPosition(position);
	SetLocalRotation(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
	SetScale(scale);
}

void ComponentTransform::InspectorDraw(std::vector<Component*> components)
{	
	float3 position = GetLocalPosition();
	float3 rotation = GetLocalRotationEuler();
	float3 scale = GetScale();

	if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
		SetLocalPosition(position);

	if (ImGui::DragFloat3("Rotation", (float*)&rotation, 0.1f))
		SetLocalRotation(rotation);

	if (ImGui::DragFloat3("Scale", (float*)&scale, 0.1f, 0.0f))
		SetScale(scale);
}

