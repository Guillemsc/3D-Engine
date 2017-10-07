#ifndef __Hierarchy_H__
#define __Hierarchy_H__

#include "EditorUI.h"
#include "Module.h"

struct ImVec4;
class Hierarchy : public EditorElement
{
public:
	Hierarchy(bool start_enabled = true);
	virtual ~Hierarchy();

	void Start();
	void Draw();

public:

private:

};

#endif