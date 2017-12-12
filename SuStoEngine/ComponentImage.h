#ifndef __COMPONENT_IMAGE_H__
#define __COMPONENT_IMAGE_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UIImage;

class ComponentCanvas : public Component
{
public:
	ComponentCanvas(GameObject* owner, std::string unique_id);
	virtual ~ComponentCanvas();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	UIImage* GetImage();

private:
	void OnEnable();
	void OnDisable();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

private:
	UIImage* image = nullptr;
};

#endif