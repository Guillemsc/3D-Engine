#include "Globals.h"
#include "App.h"
#include "Primitive.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"
#include "Primitive.h"
#include "GeometryMath.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"

DebugScene::DebugScene(bool start_enabled) : Module(start_enabled)
{
	SetName("DebugScene");
}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	return ret;
}

// Load assets
bool DebugScene::Start()
{
	bool ret = true;


	return true;
}

// Load assets
bool DebugScene::CleanUp()
{
	bool ret = true;

	return ret;
}

bool DebugScene::Update()
{
	bool ret = true;

	App->renderer3D->DrawGrid(100);

	return ret;
}

void DebugScene::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}