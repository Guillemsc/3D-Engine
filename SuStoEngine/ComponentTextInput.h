#ifndef __COMPONENT_TEXT_INPUT_H__
#define __COMPONENT_TEXT_INPUT_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UICanvas;
class UITextInput;

class ComponentTextInput : public Component
{
public:
	ComponentTextInput(GameObject* owner, std::string unique_id);
	virtual ~ComponentTextInput();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);
	UITextInput* GetText();

private:
	void OnEnable();
	void OnDisable();

	void OnChangeParent();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

	void TryBindCanvas();

private:
	UITextInput* text = nullptr;
};

#endif // !__COMPONENT_TEXT_INPUT_H__