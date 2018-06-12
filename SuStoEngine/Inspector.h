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
	void Draw(uint flags);

public:

private:
	DockContext* dock;
};

#endif