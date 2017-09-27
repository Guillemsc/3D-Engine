#ifndef __Hardware_H__
#define __Hardware_H__

#include "EditorUI.h"
#include "Module.h"

class Hardware : public EditorElement
{
public:
	Hardware(bool start_enabled = true);
	virtual ~Hardware();

	void Start();
	void Draw();

public:

private:
	string cpus;
	string sys_ram;
	string caps;

};

#endif