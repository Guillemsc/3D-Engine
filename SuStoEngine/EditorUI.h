#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"
#include "GeometryMath.h"

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
	void OnConfiguration();

private:
	void Configuration();
	void About();
	void TestEngine();
	void GeometryMathTest();
	void GenerateRandomNumbers(float2 range, int quantity);

private:
	bool show_app_about = false;

	bool show_app_configuration = false;

	bool show_imgui_test_window = false;

	bool show_test_window = false;

	// Geometry math test
	float vec3a[3] = { 0.0f, 0.0f, 0.0f };
	float dist = 0;
	bool contact = false;
	bool contact_sphere_sphere = false;
	bool contact_sphere_capsules = false;
	bool contact_aabb_aabb = false;
	bool contact_obb_obb = false;
	bool contact_aabb_ray = false;
	
	// Random generator
	float2 range_demo; // for the random generation demo
	int quantity_demo = 1; // number of random generations of the demo
	bool is_on = false;

};

#endif // __j1FILESYSTEM_H__