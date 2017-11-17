#ifndef __Configuration_H__
#define __Configuration_H__

#include "EditorUI.h"
#include "Module.h"

struct ImGuiTextEditCallbackData;
enum polygon_mode
{
	gl_fill,
	gl_line,
	gl_point
};

class Configuration : public EditorElement
{
public:
	Configuration(bool start_enabled = true);
	virtual ~Configuration();

	void Start();
	void Draw();

	void OpenGLOptions();

public:

private:
	// App
	char  name_input_buffer[255];
	char  organization_input_buffer[255];
	char  version_input_buffer[255];
	int   max_fps = 0;

	// Window
	int   window_width = 0;
	int   window_height = 0;
	float brightness = 0.0f;
	bool  fullscreen = false;
	bool  resizable = false;
	bool  borderless = false;
	bool  fulldekstop = false;
	bool  maximized = false;
	bool  vsync = false;
	int   display_size_width = 0;
	int   display_size_height = 0;

	// Camera
	float mouse_sensitivity = 0.0f;
	float wheel_speed = 0.0f;
	float camera_speed = 0.0f;

	// Rebind
	string console_binding;
	bool console_rebind = false;

	// Renderer
	bool gl_depth = true;
	bool gl_cull_face = false;
	bool gl_color_material = true;
	bool gl_texture_2d = false;
	bool fill_mode = true;
	bool wireframe_mode = false;
	bool point_mode = false;
	float point_size_slider = 1;
	bool ambient = true;
	bool diffuse = false;
	bool specular = false;
	float ambient_color[4] = { 255.0f, 255.0f, 255.0f, 255.0f };
	float diffuse_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float specular_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};

#endif