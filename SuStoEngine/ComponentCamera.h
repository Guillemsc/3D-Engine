#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* owner, std::string unique_id);
	virtual ~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();;

	Camera3D* GetCamera() const;

	void InspectorDraw(std::vector<Component*> components);

	void OnLoadAbstraction(DataAbstraction& abs);
	void OnSaveAbstraction(DataAbstraction& abs);

private:
	void OnEnable();
	void OnDisable();

private:
	Camera3D* camera = nullptr;
};

#endif
