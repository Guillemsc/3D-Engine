#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"

class EditorUI : public Module
{
public:
	EditorUI(bool enabled = true);

	// Destructor
	virtual ~EditorUI();

	bool Awake();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void ImGuiInput(SDL_Event* ev);

	void LoadStyle(char* name);

private:
	void About();
	void GeometryMathTest();

public:

private:
	bool show_app_about = false;

	// Geometry math test
	bool show_geometry_math_test = false;
	float vec3a[3] = { 0.0f, 0.0f, 0.0f };
	float dist = 0;
	bool contact = false;
	bool contact_sphere_sphere = false;
	bool contact_sphere_capsules = false;
	bool contact_aabb_aabb = false;
	bool contact_obb_obb = false;
	bool contact_aabb_ray = false;
};

#endif // __j1FILESYSTEM_H__