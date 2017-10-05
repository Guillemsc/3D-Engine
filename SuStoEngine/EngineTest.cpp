#include "EngineTest.h"
#include "App.h"
#include "imgui.h"
#include "GeometryMath.h"
#include "Primitive.h"
#include "Functions.h"
#include "Console.h"
#include "imgui_docking.h"

EngineTest::EngineTest(bool start_enabled) : EditorElement(start_enabled)
{
}

EngineTest::~EngineTest()
{
}

void EngineTest::Start()
{
	// Initial range set
	range_demo.x = 0;
	range_demo.y = 100;
}

void EngineTest::Draw()
{
	if (!visible)
		return;

	if (igBeginDock("Engine Tests", &visible, 0))
	{
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Random Number Generation"))
		{
			ImGui::InputFloat2("Min | Max", range_demo.ptr(), 2);
			ImGui::InputInt("Number of generations", &quantity_demo);

			if (ImGui::Button("Generate", { 400, 30 }))
			{
				GenerateRandomNumbers(range_demo, quantity_demo);
			}

		}
	}

	igEndDock();
}

void EngineTest::GenerateRandomNumbers(float2 range, int quantity)
{
	for (int i = 0; i < quantity; ++i)
	{
		float number_rounded = roundf(GetRandomValue(range.x, range.y) * 100) / 100;
		char number[255];
		snprintf(number, 255, "%.2f", number_rounded);
		App->editorUI->GetConsole()->AddLog(number);
	}
}

