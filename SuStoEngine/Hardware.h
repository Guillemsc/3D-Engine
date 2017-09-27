#ifndef __Hardware_H__
#define __Hardware_H__

#include "EditorUI.h"
#include "Module.h"

struct ImVec4;
class Hardware : public EditorElement
{
public:
	Hardware(bool start_enabled = true);
	virtual ~Hardware();

	void Start();
	void Draw();

public:

private:
	string sdl_ver;
	string cpus;
	string sys_ram;
	string caps;
	int    window_w;
	int    window_h;
};

#endif