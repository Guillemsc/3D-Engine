#ifndef __Game_H__
#define __Game_H__

#include "EditorUI.h"
#include "Module.h"
#include "imgui.h"

struct DockContext;

class Game : public EditorElement
{
public:
	Game(bool start_enabled = true);
	virtual ~Game();

	void Start();
	void Draw(uint flags);
	void CleanUp();

public:
	const Rect GetRect() const;

private:
	DockContext* dock;
	bool first_time = true;

	uint my_id = 0;

	float2 position;
	float2 size;
	float2 saved_size;
};

#endif