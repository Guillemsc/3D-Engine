#include "Globals.h"
#include "App.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"

DebugScene::DebugScene(bool start_enabled) : Module(start_enabled)
{
}

DebugScene::~DebugScene()
{}

bool DebugScene::Awake()
{
	bool ret = true;

	SetName("DebugScene");

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

	ImGui::ShowTestWindow();

	//ImGui::Begin("TestWindow", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	//	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders |
	//	ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);

	//if (ImGui::Button("Close App", ImVec2(200, 30)))
	//{
	//	App->EndApp();
	//}

	//ImGui::Text("Fps %f", App->GetFps());

	//ImGui::End();

	Plane p(0, 10, 0, 0);
	p.axis = true;
	p.Render();

	return ret;
}

void DebugScene::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}