#ifndef __COMPONENT_BUTTON_H__
#define __COMPONENT_BUTTON_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UICanvas;
class UIButton;

class ComponentButton : public Component
{
public:
	ComponentButton(GameObject* owner, std::string unique_id);
	virtual ~ComponentButton();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	UIButton* GetButton();

	void SetIdleImage(uint texture_id, float2 texture_size);
	void SetOverImage(uint texture_id, float2 texture_size);
	void SetPressedImage(uint texture_id, float2 texture_size);

private:
	void OnEnable();
	void OnDisable();

	void OnChangeParent();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

	void TryBindCanvas();

private:
	UIButton* button = nullptr;
	JSON_Doc config;
	bool to_load = false;
};

#endif // !_COMPONENT_BUTTON_H__