#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "GeometryMath.h"
#include "Component.h"
#include "GameObjectAbstractor.h"

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

	const void SetPosition(const float3& pos);
	const void Translate(const float3& pos);
	const void SetRotation(const float3& rotation);
	const void SetRotation(const Quat& quater);
	const void Rotate(const float3& rotate);
	const void SetScale(const float3& scale);
	const void Scale(const float3& scale);
	const float3 GetPosition() const;
	const float3 GetGlobalPosition() const;
	const float3 GetRotationEuler() const;
	const Quat GetRotationQuat() const;
	const float3 GetScale() const;

	void InspectorDraw(std::vector<Component*> components);

	void OnLoadAbstraction(DataAbstraction& abs);
	void OnSaveAbstraction(DataAbstraction& abs);

private:
	void RecalculateLocalTransform();
	void OnEnable();
	void OnDisable();

private:
	float4x4 local_transform = float4x4::identity;
	float3   local_position;
	float3   local_rotation_euler;
	Quat     local_rotation_quat;
	float3   local_scale;

	float4x4 global_transform = float4x4::identity;

};

#endif