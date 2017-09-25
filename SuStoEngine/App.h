#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Profiler.h"
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
class EditorUI;
class DebugScene;
class Configuration;

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

	void LoadConfig();

	void EndApp();
	float GetDT();
	void SetAppName(const char* name);
	void SetAppOrganization(const char* name);
	void SetMaxFps(int set);
	bool GetDebugMode();
	void SetDebugMode(bool set);

	void GoToBrowser(const char* url);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	//Modules
	XMLLoader*		   xml = nullptr;
	JSONLoader*		   json = nullptr;
	Configuration*     configuration = nullptr;
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModulePhysics3D*   physics = nullptr;
	EditorUI*		   editorUI = nullptr;
	DebugScene*        debug_scene = nullptr;

	std::list<string>  logs;

	Profiler*		   profiler = nullptr;

	std::list<Module*> modules;

private:
	int				   argc;
	char**			   args;

	std::string		   title;
	std::string		   organization;

	bool		       end_app = false;
	int				   capped_ms = -1;

	bool			   debug_mode = false;
};

extern Application* App;