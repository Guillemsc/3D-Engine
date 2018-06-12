#ifndef __ProfilerViewer_H__
#define __ProfilerViewer_H__

#include "EditorUI.h"
#include "Module.h"

class ProfilerViewer : public EditorElement
{
public:
	ProfilerViewer(bool start_enabled = true);
	virtual ~ProfilerViewer();

	void Start();
	void Draw(uint flags);
public:

private:
	int  profiler_mode = 0;
	int  ms_ticks = 0;
	bool record = false;
};

#endif