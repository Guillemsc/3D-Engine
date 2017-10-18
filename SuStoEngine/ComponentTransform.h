#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "GeometryMath.h"
#include "Component.h"

class GameObject;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* owner);
	virtual ~ComponentTransform();

	void Start();
	void Update();
	void CleanUp();

	const void SetPosition(float3& pos);
	const void SetRotationQuat(Quat& quater);

	const float3 GetPosition() const;
	const float3 GetRotationEuler() const;
	const float3 GetScale() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

public:
	float4x4 transform;
};

#endif