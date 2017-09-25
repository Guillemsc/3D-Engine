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
	strcpy(name_input_buffer, App->GetAppName());
	strcpy(organization_input_buffer, App->GetAppOrganization());
	max_fps = App->GetMaxFps();
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

			if (ImGui::InputText("Organization", organization_input_buffer, 254, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				App->SetAppOrganization(organization_input_buffer);
			}
			if (ImGui::SliderInt("Max FPS", &max_fps, 0, 999))
			{
				App->SetMaxFps(max_fps);
			}
			std::vector<float> framerate = App->profiler->GetFramesVector();
			if (!framerate.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", framerate[framerate.size() - 1]);
				ImGui::PlotHistogram("Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 500.0f, ImVec2(0, 100));
			}
		}

		// Window
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderInt("Width", &window_width, 0, 999))
			{
		
			}

			if (ImGui::SliderInt("Height", &window_height, 0, 999))
			{
	
			}
		}

	}

	ImGui::End();
}

