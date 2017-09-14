#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "DebugScene.h"
#include <cmath>

DebugScene::DebugScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

DebugScene::~DebugScene()
{}

// Load assets
bool DebugScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;



	return true;
}

// Load assets
bool DebugScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void DebugScene::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}

// Update
update_status DebugScene::Update(float dt)
{
	Plane p(0, 10, 0, 0);

	p.axis = true;

	p.Render();

	
	return UPDATE_CONTINUE;
}