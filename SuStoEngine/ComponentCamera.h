#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* owner);
	virtual ~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

private:
	Camera3D* camera = nullptr;
};

#endif
