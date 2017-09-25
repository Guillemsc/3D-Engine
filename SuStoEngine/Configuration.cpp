#include "Configuration.h"
#include "App.h"
#include "imgui.h"

Configuration::Configuration(bool start_enabled) : EditorElement(start_enabled)
{
	memset(name_input_buffer, 0, sizeof(name_input_buffer));
	memset(organization_input_buffer, 0, sizeof(organization_input_buffer));
}

Configuration::~Configuration()
{
}

void Configuration::Start()
{
	/*name_input_buffer = (char*)App->GetAppName();*/
}

void Configuration::Draw()
{
	if (!visible)
		return;

	ImGui::SetNextWindowSize(ImVec2(520, 600), 2);

	if (ImGui::Begin("Configuration", &visible))
	{
		// App
		if (ImGui::CollapsingHeader("App"))
		{
			if (ImGui::InputText("App Name", name_input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				App->SetAppName(name_input_buffer);
			}
		}

		//char title[25];
		//std::vector<float> framerate = App->profiler->GetFramesVector();
		//sprintf_s(title, 25, "Framerate %.1f", framerate[framerate.size() - 1]);
		//ImGui::PlotHistogram("Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		// Window
		ImGui::CollapsingHeader("Window");

	}

	ImGui::End();
}

