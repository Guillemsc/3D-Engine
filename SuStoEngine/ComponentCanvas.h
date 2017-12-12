#ifndef __COMPONENT_CANVAS_H__
#define __COMPONENT_CANVAS_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UICanvas;

class ComponentCanvas : public Component
{
public:
	ComponentCanvas(GameObject* owner, std::string unique_id);
	virtual ~ComponentCanvas();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	UICanvas* GetCanvas();

private:
	void OnEnable();
	void OnDisable();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

private:
	UICanvas* canvas = nullptr;
};

#endif