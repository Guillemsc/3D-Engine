#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"
#include "GeometryMath.h"

class Console;
class Configuration;
class About;
class ProfilerViewer;
class EngineTest;
class Hardware;
class DockingTest;

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
	bool DrawEditor();
	bool CleanUp();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	void ImGuiInput(SDL_Event* ev);

	void LoadStyle(char* name);

	Console* GetConsole();

private:
	void AddEditor(EditorElement* el);

private:
	Console*             console = nullptr;
	Configuration*       configuration = nullptr;
	About*               about = nullptr;
	ProfilerViewer*      profiler_viewer	= nullptr;
	EngineTest*          engine_test = nullptr;
	Hardware*			 hardware = nullptr;
	DockingTest*		 test = nullptr;

	list<EditorElement*> editor_elements;

	bool				 workspace_visible = false;

	bool                 show_imgui_test_window	= false;
	
};

#endif // __j1FILESYSTEM_H__