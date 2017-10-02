#ifndef __Test_H__
#define __Test_H__

#include "EditorUI.h"
#include "Module.h"

struct ImVec4;
struct DockContext;

class DockingTest : public EditorElement
{
public:
	DockingTest(bool start_enabled = true);
	virtual ~DockingTest();

	void Start();
	void Draw();

public:

private:
	DockContext* dock;
	bool first_time = true;

};

#endif