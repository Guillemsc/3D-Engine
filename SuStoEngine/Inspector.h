#ifndef __Inspector_H__
#define __Inspector_H__

#include "EditorUI.h"
#include "Module.h"

class DockContext;
class GameObject;

class Inspector : public EditorElement
{
public:
	Inspector(bool start_enabled = true);
	virtual ~Inspector();

	void Start();
	void Draw();

	void SetGameObjectInspected(GameObject* go);

public:
	bool is_selected = false;

	GameObject* selected_go = nullptr;

private:
	DockContext* dock;
};

#endif