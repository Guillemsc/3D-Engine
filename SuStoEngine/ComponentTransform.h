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

	float3 GetLocalPosition();
	float3 GetLocalRotationEuler();
	float3 GetLocalScale();

	void InspectorDraw();

private:
	void OnCreate();
	void OnDestroy();
	void OnEnable();
	void OnDisable();

	void ResetLocalMatrix();

public:
	float4x4 local_transform;

};

#endif