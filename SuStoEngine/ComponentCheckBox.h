#ifndef __COMPONENT_CHECKBOX_H__
#define __COMPONENT_CHECKBOX_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UICanvas;
class UICheckBox;
class JSON_Doc;

class ComponentCheckBox : public Component
{
public:
	ComponentCheckBox(GameObject* owner, std::string unique_id);
	virtual ~ComponentCheckBox();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	UICheckBox* GetCheckBox();

	void SetCheckFalseImage(uint texture_id, float2 texture_size);
	void SetCheckTrueImage(uint texture_id, float2 texture_size);

private:
	void OnEnable();
	void OnDisable();

	void OnChangeParent();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

	void TryBindCanvas();

private:
	UICheckBox* checkbox = nullptr;
	JSON_Doc config;
	bool to_load = false;
};

#endif // !_COMPONENT_CHECKBOX_H__