﻿#include "Configuration.h"
#include "App.h"
#include "ModuleWindow.h"
#include "imgui.h"
#include "mmgr\mmgr.h"
#include "SDL/include/SDL.h"
#include "glut\glut.h"
#include "imgui_docking.h"


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
	ImGui::SetNextWindowPos(ImVec2(window_width - window_width / 4, 23), 2);
	ImGui::SetNextWindowSize(ImVec2(window_width / 4, window_height - 23), 2);

	if (igBeginDock("Configuration", &visible, 0))
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

		// Render
		if (ImGui::CollapsingHeader("Render"))
		{
			OpenGLOptions();
		}
	}

	igEndDock();
}

void Configuration::OpenGLOptions()
{
	// Checkboxes
	ImGui::Checkbox("GL_DEPTH_TEST", &gl_depth);		
	
	ImGui::Checkbox("GL_LIGHTING", &gl_lighting);				ImGui::SameLine();
	ImGui::Checkbox("GL_COLOR_MATERIAL", &gl_color_material);
		// Lighting Functionalities
		if (gl_lighting)
		{
			GLenum light_type = GL_AMBIENT;
			GLenum light_num = GL_LIGHT0;
			ImGui::Combo("Lighting types", &lighting_type, "Ambient\0Diffuse\0Specular\0", 3);
			switch (lighting_type)
			{
			case 0:
				light_type_ptr = ambient;
				light_num = GL_LIGHT0;
				break;
			case 1:
				light_type_ptr = diffuse;
				light_num = GL_LIGHT1;
				break;
			case 2:
				light_type_ptr = specular;
				light_num = GL_LIGHT2;
				break;
			}
			
			ImGui::ColorPicker4("Ambient color##4", light_type_ptr, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);
			GLfloat	light_properties[] = { light_type_ptr[0], light_type_ptr[1], light_type_ptr[2], light_type_ptr[3] };

			if (ImGui::Button("Submit"))
			{
				glLightfv(light_num, light_type, light_properties);
				glEnable(light_num);
			}
			
		}
	
	ImGui::Checkbox("GL_CULL_FACE", &gl_cull_face);				ImGui::SameLine();
	ImGui::Checkbox("GL_TEXTURE_2D", &gl_texture_2d);

	ImGui::Checkbox("FILL RENDER", &fill_mode);					ImGui::SameLine();
	ImGui::Checkbox("WIREFRAME RENDER", &wireframe_mode);		ImGui::SameLine();
	ImGui::Checkbox("POINT RENDER", &point_mode);				
	//---------------------------------

	//---------------------------------
	// Slider
	if (point_mode)
	{
		ImGui::SliderFloat("Point Size", &point_size_slider, 0, 10);
		glPointSize(point_size_slider);
	}

	if (gl_depth && !glIsEnabled(GL_DEPTH_TEST))
		glEnable(GL_DEPTH_TEST);
	else if (!gl_depth && glIsEnabled(GL_DEPTH_TEST))
		glDisable(GL_DEPTH_TEST);

	if (gl_cull_face && !glIsEnabled(GL_CULL_FACE))				// https://www.khronos.org/opengl/wiki/Face_Culling
	{
		glCullFace(GL_FRONT); 									// void glCullFace(GLenum mode​); mode​ can be set to GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK.
		glEnable(GL_CULL_FACE);									// The latter will cull all triangles. Culling both faces will only cull triangles (since only they have faces).
	}
	else if (!gl_cull_face && glIsEnabled(GL_CULL_FACE))
		glDisable(GL_CULL_FACE);

	// Lighting and Color Material
	/* _________________________________________________________________GOOD SETTINGS_________________________________________________________________
	Set GL_LIGHT_0's position to something like 45 degrees to the 'vertical'. Coordinate (1,1,0) should work nicely in most cases.
	Set GL_LIGHT_0's Ambient color to 0,0,0,1
	Set GL_LIGHT_0's Diffuse color to 1,1,1,1
	Set GL_LIGHT_0's Specular color to 1,1,1,1
	Set the glLightModel global ambient to 0.2, 0.2, 0.2, 1 (this is the default).
	Don't set any other glLight or glLightModel options - just let them default.
	Enable GL_LIGHTING and GL_LIGHT_0.
	Enable GL_COLOR_MATERIAL and set glColorMaterial to GL_AMBIENT_AND_DIFFUSE.This means that glMaterial will control the polygon's specular and
	emission colours and the ambient and diffuse will both be set using glColor.
	Set the glMaterial Specular colour to 1, 1, 1, 1
	Set the glMaterial Emission colour to 0, 0, 0, 1
	Set the glColor to whatever colour you want each polygon to basically appear to be.That sets the Ambient and Diffuse to the same value - which is what you generally want.
	*/
	if (gl_lighting && !glIsEnabled(GL_LIGHTING))				// https://www.khronos.org/opengl/wiki/How_lighting_works
		glEnable(GL_LIGHTING);
	else if (!gl_lighting && glIsEnabled(GL_LIGHTING))
		glDisable(GL_LIGHTING);

	if (gl_color_material && !glIsEnabled(GL_COLOR_MATERIAL))	// https://www.khronos.org/opengl/wiki/How_lighting_works
	{
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);		// void glColorMaterial (GLenum face, GLenum mode); 
		glEnable(GL_COLOR_MATERIAL);
	}
	else if (!gl_color_material && glIsEnabled(GL_COLOR_MATERIAL))
		glDisable(GL_COLOR_MATERIAL);

	if (gl_texture_2d && !glIsEnabled(GL_TEXTURE_2D))			// https://www.khronos.org/opengl/wiki/Texture
		glEnable(GL_TEXTURE_2D);
	else if (!gl_texture_2d && glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);

	// Full 
	if (fill_mode && poly_mode != gl_fill)					// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glPolygonMode.xml
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);			// void glPolygonMode(GLenum face, GLenum mode);
		poly_mode = gl_fill;								// face:	Specifies the polygons that mode applies to. Must be GL_FRONT for front-facing polygons, 	
		wireframe_mode = false;								//			GL_BACK for back-facing polygons, or GL_FRONT_AND_BACK for front- and back-facing polygons.	
		point_mode = false;									// mode:	Specifies how polygons will be rasterized. Accepted values are GL_POINT, GL_LINE, and GL_FILL.
	} 														//			The initial value is GL_FILL for both front- and back-facing polygons.	

															// Wireframe											
	if (wireframe_mode && poly_mode != gl_line)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		poly_mode = gl_line;
		fill_mode = false;
		point_mode = false;
	}

	// Point											
	if (point_mode && poly_mode != gl_point)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		poly_mode = gl_point;
		fill_mode = false;
		wireframe_mode = false;
	}

	if (!point_mode && !wireframe_mode && !fill_mode)
		fill_mode = true;
}