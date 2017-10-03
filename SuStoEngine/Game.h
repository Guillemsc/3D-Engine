#ifndef __Game_H__
#define __Game_H__

#include "EditorUI.h"
#include "Module.h"

struct ImVec4;
struct DockContext;

class Game : public EditorElement
{
public:
	Game(bool start_enabled = true);
	virtual ~Game();

	void Start();
	void Draw();

public:

private:
	DockContext* dock;
	bool first_time = true;

};

#endif