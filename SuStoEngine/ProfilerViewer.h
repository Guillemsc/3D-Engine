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
	void Draw();

public:

private:

};

#endif