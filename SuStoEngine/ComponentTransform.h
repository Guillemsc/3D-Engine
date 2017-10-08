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

	float3 GetPosition();
	float3 GetRotation();
	float3 GetScale();

	void InspectorDraw();

private:
	void OnCreate();
	void OnDestroy();
	void OnEnable();
	void OnDisable();

public:
	float4x4 transform;

};

#endif