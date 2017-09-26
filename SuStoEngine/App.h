#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Profiler.h"
#include "Module.h"

class Profiler;
class JSON_Doc;

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
	void SaveConfig(Module* module = nullptr);
	JSON_Doc* GetConfig();

	void EndApp();
	float GetDT();
	void SetAppName(const char* name);
	const char* GetAppName();
	void SetAppOrganization(const char* name);
	const char* GetAppOrganization();
	void SetMaxFps(int set);
	int GetMaxFps();
	void SetDebugMode(bool set);
	bool GetDebugMode();
	void SetVersion(const char* version);
	const char* GetVersion();

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
	std::string		   version;

	bool		       end_app = false;
	int				   max_fps = 0;
	int				   capped_ms = -1;

	bool			   debug_mode = false;

	JSON_Doc*          config = nullptr;
};

extern Application* App;