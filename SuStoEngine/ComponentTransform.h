#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "GeometryMath.h"
#include "Component.h"

class GameObject;

class ComponentTransform : public Component
{
	friend class GameObject;

public:
	ComponentTransform(GameObject* owner, std::string unique_id);
	virtual ~ComponentTransform();

	void Start();
	void Update();
	void CleanUp();

	const float4x4 GetLocalTransform() const;
	const void SetLocalTransform(const float4x4& transform);
	const float4x4 GetGlobalTransform() const;
	const void SetGlobalTransform(const float4x4& transform);

	const void SetLocalPosition(const float3& pos);
	const void SetLocalRotation(const float3& rotation);
	const void SetLocalRotation(const Quat& quater);
	const void SetScale(const float3& scale);
	const void SetLocalPosRotScal(const float3& pos, const Quat& quater, const float3& scale);

	const void Translate(const float3& pos);
	const void Rotate(const float3& rotate);
	const void Scale(const float3& scale);

	const float3 GetLocalPosition() const;
	const float3 GetGlobalPosition() const;
	const float3 GetLocalRotationEuler() const;
	const Quat GetLocalRotationQuat() const;
	const float3 GetScale() const;

	void InspectorDraw(std::vector<Component*> components);

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

private:
	void RecalculateLocalTransform();
	void OnEnable();
	void OnDisable();

private:
	float4x4 local_transform = float4x4::identity;
	float4x4 local_position = float4x4::identity;
	float3   local_rotation_euler;
	float4x4     local_rotation_quat;
	float4x4 local_scale = float4x4::identity;

	float4x4 global_transform = float4x4::identity;

};

#endif