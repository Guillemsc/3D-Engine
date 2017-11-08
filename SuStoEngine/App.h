#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Profiler.h"
#include "IDGenerator.h"
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
class ModuleGameObject;
class EditorUI;
class Configuration;
class GeometryLoader;
class TextureLoader;
class FileSystem;
class SceneManager;
class ResourceManager;

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
	JSON_Doc* GetConfig() const;

	void LoadFile(const char* filename);

	void EndApp();
	float GetDT() const;
	void SetAppName(const char* name);
	const char* GetAppName() const;
	void SetAppOrganization(const char* name);
	const char* GetAppOrganization();
	void SetMaxFps(int const &set);
	int GetMaxFps() const;
	void SetDebugMode(bool set);
	bool GetDebugMode() const;
	void SetVersion(const char* version);
	const char* GetVersion();
	SDL_version GetSDLVersion();
	const char* GetBasePath();

	void GoToBrowser(const char* url);
	void GoToFolder(const char* folder);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	//Modules
	XMLLoader*		   xml = nullptr;
	JSONLoader*		   json	= nullptr;
	Configuration*     configuration = nullptr;
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModuleGameObject*  gameobj = nullptr;
	EditorUI*		   editorUI	= nullptr;
	GeometryLoader*	   geometry = nullptr;
	TextureLoader*	   texture = nullptr;
	FileSystem*		   file_system = nullptr;
	SceneManager*	   scene_manager = nullptr;
	ResourceManager*   resource_manager = nullptr;

	std::list<string>  logs;

	Profiler*		   profiler = nullptr;
	IDGen*		       id = nullptr;

	std::list<Module*> modules;

private:
	int				   argc;
	char**			   args;

	std::string		   title = "";
	std::string		   organization = "";
	std::string		   version = "";

	bool		       end_app = false;

	bool			   debug_mode = false;

	JSON_Doc*          config = nullptr;
};

extern Application* App;