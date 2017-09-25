#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"
#include "GeometryMath.h"

class Console;
class Configuration;
class About;

class EditorElement
{
public:
	EditorElement(bool start_enabled = true) { visible = start_enabled; };
	virtual ~EditorElement() {};

	virtual void Start() {};
	virtual void Draw() {};
	virtual void CleanUp() {};

	bool visible = false;
};

class EditorUI : public Module
{
public:
	EditorUI(bool enabled = true);

	// Destructor
	virtual ~EditorUI();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void ImGuiInput(SDL_Event* ev);

	void LoadStyle(char* name);

private:
	void AddEditor(EditorElement* el);

	void TestEngine();
	void GeometryMathTest();
	void GenerateRandomNumbers(float2 range, int quantity);

private:
	Console* console = nullptr;
	Configuration* configuration = nullptr;
	About* about = nullptr;

	list<EditorElement*> editor_elements;

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
};

#endif // __j1FILESYSTEM_H__