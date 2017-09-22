#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"

class Profiler;

class XMLLoader;
class JSONLoader;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class Console;
class EditorUI;
class DebugScene;

class Application
{
public:
	Application(int argc, char* args[]);
	~Application();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	int GetArgc() const;
	const char* GetArgv(int index) const;

	void EndApp();
	float GetDT();
	int GetFps();
	float GetAvgFps();
	int GetFramesSinceStart();
	bool GetDebugMode();
	void SetDebugMode(bool set);

	void GoToBrowser(const char* url);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void FrameCalculations();

public:
	//Modules
	XMLLoader*		   xml = nullptr;
	JSONLoader*		   json = nullptr;
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModulePhysics3D*   physics = nullptr;
	EditorUI*		   editorUI = nullptr;
	Console*		   console = nullptr;
	DebugScene*        debug_scene = nullptr;

	std::list<string>  logs;

	Profiler*		   profiler = nullptr;

private:
	std::list<Module*> modules;

	int				   argc;
	char**			   args;

	std::string		   title;
	std::string		   organization;

	bool		       end_app = false;
	bool			   debug_mode = false;
	int				   capped_ms = -1;
};

extern Application* App;