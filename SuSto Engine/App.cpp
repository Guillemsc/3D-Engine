#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "DebugScene.h"
#include "FileSystem.h"
#include "XMLLoader.h"

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	fs = new FileSystem();
	xml = new XMLLoader();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	debug_scene = new DebugScene();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(fs);
	AddModule(xml);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(debug_scene);

	// Renderer last
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		RELEASE(*it);
	}
}

bool Application::Awake()
{
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Awake();
		if (!ret) return false;
	}

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Start();
		if (!ret) return false;
	}

	startup_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = (float)startup_time.Read() / 1000.0f;
	startup_time.Start();
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;

	if (input->GetWindowEvent(WE_QUIT) == true || end_app)
		return false;

	PrepareUpdate();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->PreUpdate();

		if (!ret) return false;
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->Update();

		if (!ret) return false;
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->PostUpdate();

		if (!ret) return false;
	}

	FinishUpdate();

	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	FrameCalculations();
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
	{
		ret = (*it)->CleanUp();

		if (!ret) return false;
	}

	return ret;
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

int Application::GetArgc() const
{
	return argc;
}

const char * Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

void Application::EndApp()
{
	end_app = true;
}

float Application::GetDT()
{
	return dt;
}

float Application::GetFps()
{
	return frames_on_last_update;
}

float Application::GetAvgFps()
{
	return avg_fps;
}

int Application::GetFramesSinceStart()
{
	return frame_count;
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}