#include "Configuration.h"
#include "App.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "mmgr\mmgr.h"
#include "SDL/include/SDL.h"
#include "imgui_docking.h"
#include <gl\GLU.h>
#include <gl\GL.h>

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
	vsync = App->window->GetVsync();
	App->window->GetWindowSize(display_size_width, display_size_height);
	console_binding = App->input->GetKeyBinding("console");
	mouse_sensitivity = App->camera->GetMouseSensitivity();
	wheel_speed = App->camera->GetWheelSpeed();
	camera_speed = App->camera->GetCameraSpeed();
}

void Configuration::Draw(uint flags)
{
	if (App->input->GetKeyBindingDown("configuration"))
	{
		visible = !visible;
	}

	ImGuiStyle * style = &ImGui::GetStyle();
	ImVec4 sec_colour = style->Colors[ImGuiCol_ComboBg];
	sec_colour.x += +0.2f;
	sec_colour.y += +0.2f;
	sec_colour.z += +0.2f;

	App->window->GetWindowSize(window_width, window_height);
	ImGui::SetNextWindowPos(ImVec2(window_width - window_width / 4, 23), 2);
	ImGui::SetNextWindowSize(ImVec2(window_width / 4, window_height - 23), 2);

	ImGui::BeginDock("Configuration", false, &visible, false, flags);
	
	// ---------------------------------------------------------------------
	// App -----------------------------------------------------------------
	// ---------------------------------------------------------------------
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
	}

	// ---------------------------------------------------------------------
	// Window --------------------------------------------------------------
	// ---------------------------------------------------------------------
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
		ImGui::SameLine();
		if (ImGui::Checkbox("VSync", &vsync))
		{
			App->window->SetVsync(vsync);
			App->SaveConfig(App->window);
		}
	}

	// ---------------------------------------------------------------------
	// Renderer ------------------------------------------------------------
	// ---------------------------------------------------------------------
	if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
	{
		OpenGLOptions();
	}

	// ---------------------------------------------------------------------
	// Input ---------------------------------------------------------------
	// ---------------------------------------------------------------------
	if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Mouse: x:%d, y:%d", App->input->GetMouseX(), App->input->GetMouseY());



		ImGui::Separator();
		// Console
		ImGui::TextColored(sec_colour, "[ %s ]", console_binding.c_str());
		ImGui::SameLine();
		if (ImGui::SmallButton("Rebind"))
		{
			console_rebind = true;
		}
		ImGui::SameLine();
		ImGui::Text("Console"); 

		if (console_rebind)
		{
			console_binding = "PREESS A KEY";

			string input;
			if (App->input->GetKeyboardInput(input))
			{
				console_rebind = false;
				console_binding = ToUpperCase(input);
				App->input->SetKeyBinding(console_binding.c_str(), "console");
				App->input->ClearKeyboardInput();
				App->SaveConfig(App->input);
			}
		}

		// Configuration

		// Profiler
	}

	// ---------------------------------------------------------------------
	// Camera --------------------------------------------------------------
	// ---------------------------------------------------------------------
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat("Mouse sensitivity", &mouse_sensitivity, 0.01f, 1))
		{
			App->camera->SetMouseSensitivity(mouse_sensitivity);
			App->SaveConfig(App->camera);
		}

		if (ImGui::SliderFloat("Wheel speed", &wheel_speed, 0.01f, 200))
		{
			App->camera->SetWheelSpeed(wheel_speed);
			App->SaveConfig(App->camera);
		}

		if (ImGui::SliderFloat("Camera speed", &camera_speed, 0.01f, 500))
		{
			App->camera->SetCameraSpeed(camera_speed);
			App->SaveConfig(App->camera);
		}
	}
	

	ImGui::EndDock();
}

void Configuration::OpenGLOptions()
{
	// Checkboxes
	if (ImGui::Checkbox("Depht test", &gl_depth))
	{
		App->renderer3D->SetDepthTest(gl_depth);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Color material", &gl_color_material))
	{
		App->renderer3D->SetColorMaterial(gl_color_material);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Cull face", &gl_cull_face))
	{
		App->renderer3D->SetCullFace(gl_cull_face);
	}
	ImGui::Separator();
	if (ImGui::Checkbox("Fill render", &fill_mode))
	{
		wireframe_mode = false;
		point_mode = false;
		App->renderer3D->SetPoligonModeFill();
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Wireframe render ", &wireframe_mode))
	{
		fill_mode = false;
		point_mode = false;
		App->renderer3D->SetPoligonModeWireframe();
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Point render", &point_mode))
	{
		fill_mode = false;
		wireframe_mode = false;
		App->renderer3D->SetPoligonModePoints();
	}

	if (point_mode)
	{
		ImGui::SliderFloat("Point Size", &point_size_slider, 0, 10);
		App->renderer3D->SetPoligonModePoints(point_size_slider);
	}
	ImGui::Separator();
	if(ImGui::Checkbox("Ambient", &ambient))
	{
		App->renderer3D->SetAmbientLight(ambient, ambient_color);
	}

	if (ambient)
	{
		ImGui::ColorPicker4("Ambient color##4", ambient_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);
		App->renderer3D->SetAmbientLight(ambient, ambient_color);
	}

	if (ImGui::Checkbox("Diffuse", &diffuse))
	{
		App->renderer3D->SetDiffuseLight(diffuse, diffuse_color);
	}

	if (diffuse)
	{
		ImGui::ColorPicker4("Difuse color##4", diffuse_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);
		App->renderer3D->SetDiffuseLight(diffuse, diffuse_color);
	}

	if (ImGui::Checkbox("Specular", &specular))
	{
		App->renderer3D->SetSpecularLight(specular, specular_color);
	}

	if (specular)
	{
		ImGui::ColorPicker4("Specular color##4", specular_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);
		App->renderer3D->SetSpecularLight(specular, specular_color);
	}	
}