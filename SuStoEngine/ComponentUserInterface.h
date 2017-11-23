#ifndef __COMPONENT_USER_INTERFACE_H__
#define __COMPONENT_USER_INTERFACE_H__

#include "Component.h"

class GameObject;

enum UIElements
{
	UI_CANVAS,
	UI_PANEL,
	UI_IMAGE,
	UI_TEXT,
	UI_BUTTON,
	UI_TEXT_INPUT,
	UI_CHECKBOX,
	
	UI_NULL
};

class ComponentUserInterface : public Component
{
public:
	ComponentUserInterface(GameObject* owner, std::string unique_id);
	virtual ~ComponentUserInterface();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

private:
	UIElements ui_type = UIElements::UI_NULL;

public:

};

#endif