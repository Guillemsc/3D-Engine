#ifndef __About_H__
#define __About_H__

#include "EditorUI.h"
#include "Module.h"

class About : public EditorElement
{
public:
	About(bool start_enabled = true);
	virtual ~About();

	void Start();
	void Draw(uint flags);

public:

private:

};

#endif