#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "GeometryMath.h"
#include "Component.h"

class GameObject;

class ComponentTransfrom : public Component
{
public:
	ComponentTransfrom(GameObject* owner);
	virtual ~ComponentTransfrom();

	void Start();
	void Update();
	void CleanUp();

	float3 GetPosition();
	float3 GetRotationEuler();
	float3 GetScale();

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

	void ResetTransformMatrix();

public:
	float4x4 transform;

};

#endif