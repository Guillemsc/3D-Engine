#pragma once

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

class KDTree;

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
	KDTree* kdtree = nullptr;
private:



};
