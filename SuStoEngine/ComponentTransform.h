#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "GeometryMath.h"
#include "Component.h"

class GameObject;

class ComponentTransform : public Component
{
	friend class GameObject;

public:
	ComponentTransform(GameObject* owner);
	virtual ~ComponentTransform();

	void Start();
	void Update();
	void CleanUp();

	const float4x4 GetLocalTransform() const;
	const void SetLocalTransform(const float4x4& transform);
	const float4x4 GetGlobalTransform() const;

	const void SetPosition(float3& pos);
	const void SetRotation(const Quat& quater);
	const float3 GetPosition() const;
	const float3 GetRotationEuler() const;
	const float3 GetScale() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

private:
	float4x4 local_transform;
	float4x4 global_transform;
};

#endif