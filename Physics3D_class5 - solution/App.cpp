#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "DebugScene.h"
#include <list>

Application::Application(int argc, char* args[])
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	debug_scene = new DebugScene();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(debug_scene);


	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		RELEASE(*it);
	}
}

bool Application::Awake()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Awake();
	}

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Start();
	}

	startup_time.Start();
	return true;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)startup_time.Read() / 1000.0f;
	startup_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

void Application::FrameCalculations()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / startup_time.ReadSec();
	seconds_since_startup = startup_time.ReadSec();
	last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	PrepareUpdate();
	
	if (ret)
	{
		for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
		{
			ret = (*it)->PreUpdate();
		}
	}

	if (ret)
	{
		for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
		{
			ret = (*it)->Update();
		}
	}

	if (ret)
	{
		for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
		{
			ret = (*it)->PostUpdate();
		}
	}

	FinishUpdate();

	FrameCalculations();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

float Application::GetDT()
{
	return dt;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}