#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include "EditorUI.h"
#include "Module.h"

class Explorer : public EditorElement
{
public:
	Explorer(bool start_enabled = true);
	virtual ~Explorer();

	void Start();
	void Draw();

public:

private:

};

#endif