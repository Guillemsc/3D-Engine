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
class Game;

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

	void LoadLayoutsInfo();
	void SaveLayoutsInfo();
	void AddExistingLayout(const char* layout);
	void SetCurrentLayout(const char* current = "");
	void LoadCurrentLayout();
	void SaveCurrentLayout();
	void SaveNewLayout(const char* layout);

private:
	void AddEditor(EditorElement* el);

private:
	JSON_Doc*			 layout = nullptr;
	std::list<string>	 layouts;
	string				 current_layout;

	Console*             console = nullptr;
	Configuration*       configuration = nullptr;
	About*               about = nullptr;
	ProfilerViewer*      profiler_viewer	= nullptr;
	EngineTest*          engine_test = nullptr;
	Hardware*			 hardware = nullptr;
	Game*				 game = nullptr;

	list<EditorElement*> editor_elements;

	bool				 workspace_visible = false;

	bool                 show_imgui_test_window	= false;
};

#endif // __j1FILESYSTEM_H__