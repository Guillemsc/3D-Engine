#ifndef __COMPONENT_BUTTON_H__
#define __COMPONENT_BUTTON_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Camera3D;
class UICanvas;
class UIText;

class ComponentText : public Component
{
public:
	ComponentText(GameObject* owner, std::string unique_id);
	virtual ~ComponentText();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	UIText* GetText();

	//void SetIdleImage(uint texture_id, float2 texture_size);
	

private:
	void OnEnable();
	void OnDisable();

	void OnChangeParent();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

	void TryBindCanvas();

private:
	UIText* text = nullptr;
};

#endif // !_COMPONENT_BUTTON_H__