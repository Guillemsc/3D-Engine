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

private:
	DockContext* dock;
	bool create_empty_game_object = false;
	bool create_cube_object = false;

	bool first_time_selected = true;
};

#endif