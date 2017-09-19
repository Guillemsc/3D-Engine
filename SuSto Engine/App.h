#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "Math.h" // Includes everything MathGeoLib related
#include "MathGeoLib\include\Algorithm\Random\LCG.h" // Includes random number generators

class FileSystem;
class XMLLoader;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
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
	float GetFps();
	float GetAvgFps();
	int GetFramesSinceStart();

	void GoToBrowser(string web);

private:
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	void FrameCalculations();

public:
	//Modules
	FileSystem*		   fs = nullptr;
	XMLLoader*		   xml = nullptr;
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModulePhysics3D*   physics = nullptr;
	EditorUI*		   editorUI = nullptr;
	DebugScene*        debug_scene = nullptr;

private:
	std::list<Module*>      modules;

	int				   argc;
	char**			   args;

	std::string		   title;
	std::string		   organization;

	bool		       end_app = false;
	// Engine debug info
	int				   capped_ms = -1;
	PerfTimer		   ptimer;
	int				   frame_count = 0;
	Timer			   startup_time;
	Timer			   frame_time;
	Timer			   last_sec_frame_time;
	int				   last_sec_frame_count = 0;
	int				   prev_last_sec_frame_count = 0;
	float		   	   dt = 0.0f;
	float			   avg_fps = 0;
	float			   seconds_since_startup = 0;
	float			   last_frame_ms = 0;
	float			   frames_on_last_update = 0;
};

extern Application* App;