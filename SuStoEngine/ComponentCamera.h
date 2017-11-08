#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;

class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* owner, double id);
	virtual ~ComponentCamera();

	void Start();
	void Update();
	void CleanUp();;

	Camera3D* GetCamera() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

	virtual void OnLoadScene(JSON_Doc* config);
	virtual void OnSaveScene(JSON_Doc* config);

private:
	Camera3D* camera = nullptr;
};

#endif
