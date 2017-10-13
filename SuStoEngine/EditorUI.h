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
class Hierarchy;
class Inspector;

class ImVec4;

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

	void ImGuiInput(SDL_Event* const ev);

	void LoadStyle(const char* name);

	Console* GetConsole() const;
	Inspector* GetInspector() const;

	void LoadLayoutsInfo();
	void SaveLayoutsInfo();
	
	void AddExistingLayout(const char* layout);
	void SetCurrentLayout(const char* current = "");

	void LoadCurrentLayout();
	void SaveCurrentLayout();
	void SaveNewLayout(const char* layout);

	void RemoveLayout(const char* layout);

	const float4 GameRect() const;

private:
	void AddEditor(EditorElement* el);

private:
	Console*             console = nullptr;
	Configuration*       configuration = nullptr;
	About*               about = nullptr;
	ProfilerViewer*      profiler_viewer	= nullptr;
	EngineTest*          engine_test = nullptr;
	Hardware*			 hardware = nullptr;
	Game*				 game = nullptr;
	Hierarchy*			 hierarchy = nullptr;
	Inspector*			 inspector = nullptr;

	vector<EditorElement*> editor_elements;

	JSON_Doc*			 layout = nullptr;
	std::list<string>	 layouts;
	string				 current_layout;

	bool				 workspace_visible = false;

	bool                 show_imgui_test_window	= false;

	char				 layout_name[255];
};

#endif // __j1FILESYSTEM_H__