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
#include "ModuleGameObject.h"
#include "ComponentCamera.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

DebugScene::DebugScene(bool start_enabled) : Module(start_enabled)
{
	SetName("DebugScene");
}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	GameObject* go = App->gameobj->Create();
	go->AddComponent(CAMERA);
	ComponentCamera* cam = (ComponentCamera*)go->FindComponentByType(CAMERA);
	cam->GetCamera()->SetFarPlaneDistance(100.0f);

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

	//App->renderer3D->GetDebugDraw()->DrawCone(float3(-6, 10, 0));

	return ret;
}