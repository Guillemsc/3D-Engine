#include "Globals.h"
#include "App.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "DebugScene.h"
#include <cmath>
#include "imgui.h"

DebugScene::DebugScene(bool start_enabled) : Module( start_enabled)
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

	ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::BulletText("Click and drag on lower right corner to resize window.");
	ImGui::BulletText("Click and drag on any empty space to move window.");
	ImGui::BulletText("Mouse Wheel to scroll.");
	if (ImGui::GetIO().FontAllowUserScaling)
		ImGui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
	ImGui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
	ImGui::BulletText("CTRL+Click on a slider or drag box to input text.");
	ImGui::BulletText(
		"While editing text:\n"
		"- Hold SHIFT or use mouse to select text\n"
		"- CTRL+Left/Right to word jump\n"
		"- CTRL+A or double-click to select all\n"
		"- CTRL+X,CTRL+C,CTRL+V clipboard\n"
		"- CTRL+Z,CTRL+Y undo/redo\n"
		"- ESCAPE to revert\n"
		"- You can apply arithmetic operators +,*,/ on numerical values.\n"
		"  Use +- to subtract.\n");

	Plane p(0, 10, 0, 0);
	p.axis = true;
	p.Render();

	return ret;
}

void DebugScene::OnCollision(PhysBody3D * body1, PhysBody3D * body2)
{
}