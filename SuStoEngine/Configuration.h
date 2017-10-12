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

	// OpenGL Options
	bool gl_depth = true;
	bool gl_cull_face = false;
	bool gl_color_material = true;
	bool gl_texture_2d = false;
	bool fill_mode = true;
	bool wireframe_mode = false;
	bool point_mode = false;
	polygon_mode poly_mode = polygon_mode::gl_fill;
	float point_size_slider = 1;

	int lighting_type = 0;
	float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diffuse[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float* light_type_ptr = ambient;

};

#endif