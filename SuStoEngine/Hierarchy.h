#ifndef __Hierarchy_H__
#define __Hierarchy_H__

#include "EditorUI.h"
#include "Module.h"

class DockContext;
class GameObject;

class Hierarchy : public EditorElement
{
public:
	Hierarchy(bool start_enabled = true);
	virtual ~Hierarchy();

	void Start();
	void Draw();

public:
	std::list<GameObject*> game_objects;

private:
	DockContext* dock;

};

#endif