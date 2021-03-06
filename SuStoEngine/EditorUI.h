#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "Module.h"
#include "GeometryMath.h"

class MainBar;
class ToolsBar;
class Console;
class Configuration;
class About;
class ProfilerViewer;
class EngineTest;
class Hardware;
class Game;
class Hierarchy;
class Inspector;
class Explorer;
class ResourcesMonitoring;

class ImVec4;

class EditorElement
{
public:
	EditorElement(bool start_enabled = true) { visible = start_enabled; };
	virtual ~EditorElement() {};

	virtual void Start() {};
	virtual void Draw(uint flags) {};
	virtual void CleanUp() {};

	void SetInteractable(bool set);
	bool GetInteractable() const;

public:
	bool visible = false;

protected:
	bool interactable = true;
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
	bool DrawEditor();
	bool CleanUp();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	void LoadScene(JSON_Doc* config);
	void SaveScene(JSON_Doc* config);

	void ImGuiInput(SDL_Event* const ev);

	void LoadStyle(const char* name);

	void LoadLayoutsInfo();
	void SaveLayoutsInfo();
	
	void SetCurrentLayout(const char* current = "");
	string GetCurrentLayout();
	std::list<string> GetLayouts();

	void LoadCurrentLayout();
	void SaveCurrentLayout();
	void SaveNewLayout(const char* layout);
	void RemoveLayout(const char* layout);

	const Rect GameRect() const;

	MainBar* GetMainBar();
	ToolsBar* GetToolsBar();
	Console* GetConsole();
	Configuration* GetConfiguration();
	About* GetAbout();
	ProfilerViewer* GetProfilerViewer();
	Hardware* GetHardware();
	Game* GetGame();
	Hierarchy* GetHerarchy();
	Inspector* GetInspector();
	Explorer* GetExplorer();
	ResourcesMonitoring* GetResourcesMonitoring();

	void SetEditorInteractable(bool set);

private:
	void AddEditor(EditorElement* el);
	void AddExistingLayout(const char* layout);
	void DrawDockingEditor();

private:
	MainBar*		     main_bar = nullptr;
	ToolsBar*			 tools_bar = nullptr;
	Console*             console = nullptr;
	Configuration*       configuration = nullptr;
	About*               about = nullptr;
	ProfilerViewer*      profiler_viewer	= nullptr;
	EngineTest*          engine_test = nullptr;
	Hardware*			 hardware = nullptr;
	Game*				 game = nullptr;
	Hierarchy*			 hierarchy = nullptr;
	Inspector*			 inspector = nullptr;
	Explorer*			 explorer = nullptr;
	ResourcesMonitoring* resources_monitoring = nullptr;

	vector<EditorElement*> editor_elements;

	JSON_Doc*			 layout = nullptr;
	std::list<string>	 layouts;
	string				 current_layout;

	bool				 workspace_visible = false;

	bool                 show_imgui_test_window	= false;

	char				 layout_name[255];
};

#endif // __j1FILESYSTEM_H__