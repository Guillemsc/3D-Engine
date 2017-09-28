#include "Configuration.h"
#include "App.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "mmgr\mmgr.h"
#include "SDL/include/SDL.h"

Configuration::Configuration(bool start_enabled) : EditorElement(start_enabled)
{

}

Configuration::~Configuration()
{
}

void Configuration::Start()
{
	strcpy(name_input_buffer, App->GetAppName());
	strcpy(organization_input_buffer, App->GetAppOrganization());
	strcpy(version_input_buffer, App->GetVersion());
	max_fps = App->GetMaxFps();
	App->window->GetWindowSize(window_width, window_height);
	brightness = App->window->GetBrightness();
	fullscreen = App->window->GetFullscreen();
	resizable = App->window->GetResizable();
	borderless = App->window->GetBorderless();
	fulldekstop = App->window->GetFullDekstop();
	maximized = App->window->GetMaximized();
	App->window->GetWindowSize(display_size_width, display_size_height);
}

void Configuration::Draw()
{
	if (!visible)
		return;

	App->window->GetWindowSize(window_width, window_height);

	ImGui::SetNextWindowSize(ImVec2(400, 800), 2);

	if (ImGui::Begin("Configuration", &visible))
	{
		// App
		if (ImGui::CollapsingHeader("App", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::InputText("App Name", name_input_buffer, 254))
			{
				App->SetAppName(name_input_buffer);
				App->SaveConfig();
			}

			if (ImGui::InputText("Organization", organization_input_buffer, 254))
			{
				App->SetAppOrganization(organization_input_buffer);
				App->SaveConfig();
			}
			if (ImGui::InputText("Version", version_input_buffer, 254))
			{
				App->SetVersion(version_input_buffer);
				App->SaveConfig();
			}
			if (ImGui::SliderInt("Max FPS", &max_fps, 0, 999))
			{
				App->SetMaxFps(max_fps);
				App->SaveConfig();
			}
			ImGui::Text("Average fps: %d fps", (int)App->profiler->GetAvgFPS());
			std::vector<float> framerate = App->profiler->GetFramesVector();

			if (!framerate.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", framerate[framerate.size() - 1]);
				ImGui::PlotHistogram("Framerate", &framerate[0], framerate.size(), 0, title, 0.0f, 500.0f, ImVec2(0, 100));
			}

			std::vector<float> memory = App->profiler->GetMemoryVector();

			if (!memory.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", memory[memory.size() - 1]);
				ImGui::PlotHistogram("Memory", &memory[0], memory.size(), 0, title, 0.0f, 30000.0f, ImVec2(0, 100));
			}
		}

		// Window
		if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderFloat("Brightness", &brightness, 0, 1))
			{
				App->window->SetBrightness(brightness);
				App->SaveConfig(App->window);
			}
			if (ImGui::InputInt("Width", &window_width, 100))
			{
				if (window_width > display_size_width)
					window_width = display_size_width;

				App->window->SetWindowSize(window_width, window_height);
				App->SaveConfig(App->window);
			}
			if (ImGui::InputInt("Height", &window_height, 100))
			{
				if (window_height > display_size_height)
					window_height = display_size_height;

				App->window->SetWindowSize(window_width, window_height);
				App->SaveConfig(App->window);
			}
			if (ImGui::Checkbox("Fullscren", &fullscreen))
			{
				App->window->SetFullscreen(fullscreen);
				App->SaveConfig(App->window);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
			{
				App->window->SetResizable(resizable);
				App->SaveConfig(App->window);
			}
			if (ImGui::Checkbox("Borderless", &borderless))
			{
				App->window->SetBorderless(borderless);
				App->SaveConfig(App->window);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Dekstop", &fulldekstop))
			{
				App->window->SetFullDekstop(fulldekstop);
				App->SaveConfig(App->window);
			}

			maximized = App->window->GetMaximized();
			if (ImGui::Checkbox("Maximized", &maximized))
			{
				App->window->SetMaximized(maximized);
				App->SaveConfig(App->window);
			}
		}

	}

	ImGui::End();
}



