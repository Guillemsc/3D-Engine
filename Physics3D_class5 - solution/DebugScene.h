#pragma once

#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List2.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class DebugScene : public Module
{
public:
	DebugScene(Application* app, bool start_enabled = true);
	~DebugScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

public:


};
