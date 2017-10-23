#pragma once

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"


class DebugScene : public Module
{
public:
	DebugScene(bool start_enabled = true);
	~DebugScene();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

public:

private:



};
