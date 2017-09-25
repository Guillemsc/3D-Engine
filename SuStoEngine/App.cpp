#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "DebugScene.h"
#include "XMLLoader.h"
#include "JSONLoader.h"
#include "EditorUI.h"
#include "Configuration.h"
#include "imgui.h"

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	xml = new XMLLoader();
	json = new JSONLoader();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	debug_scene = new DebugScene();
	editorUI = new EditorUI();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(xml);
	AddModule(json);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(editorUI);

	AddModule(debug_scene);

	// Renderer last
	AddModule(renderer3D);

	profiler = new Profiler();
	SetDebugMode(true);
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

	LoadConfig();

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		ret = (*it)->Awake();
		if (!ret) return false;
	}

	profiler->AwakeFinish();

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

	profiler->StartFinish();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;

	if (input->GetWindowEvent(WE_QUIT) == true || end_app)
		return false;

	// Cap fps
	if (capped_ms > 0 && GetDT() < capped_ms)
	{
		SDL_Delay(capped_ms - GetDT());
	}

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

		profiler->StartProfile((*it)->GetName());

		ret = (*it)->Update();

		profiler->FinishProfile();

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

	profiler->UpdateFinish();

	return ret;
}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
	{
		ret = (*it)->CleanUp();

		if (!ret) return false;
	}

	profiler->CleanUp();

	return ret;
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

void Application::LoadConfig()
{
	config = json->LoadJSON("config.json");

	if (config != nullptr)
	{
		const char* title = config->GetString("app.title");
		const char* organization = config->GetString("app.organization");
		const char* version = config->GetString("app.version");
		int max_fps = config->GetNumber("app.max_fps");
		
		SetAppName(title);
		SetAppOrganization(organization);
		SetVersion(version);
		SetMaxFps(max_fps);
	}
}

JSON_Doc* Application::GetConfig()
{
	return config;
}

void Application::EndApp()
{
	end_app = true;
}

float Application::GetDT()
{
	return profiler->GetFrameTime()/1000;
}

void Application::SetAppName(const char* name)
{
	if (title != name)
	{
		title = name;
		window->SetTitle(name);

		if (config != nullptr)
		{
			config->SetString("app.title", name);
			config->Save();
		}
	}
}

const char * Application::GetAppName()
{
	return title.c_str();
}

void Application::SetAppOrganization(const char* name)
{
	if (name != organization)
	{
		organization = name;

		if (config != nullptr)
		{
			config->SetString("app.organization", name);
			config->Save();
		}
	}
}

const char * Application::GetAppOrganization()
{
	return organization.c_str();
}

void Application::SetMaxFps(int set)
{
	if (set > 0)
	{
		max_fps = set;
		capped_ms = (1000 / set);

		if (config != nullptr)
		{
			config->SetNumber("app.max_fps", set);
			config->Save();
		}
	}
}

int Application::GetMaxFps()
{
	return max_fps;
}

bool Application::GetDebugMode()
{
	return debug_mode;
}

void Application::SetVersion(const char * set)
{
	version = set;

	if (config != nullptr)
	{
		config->SetString("app.version", set);
		config->Save();
	}
}

const char * Application::GetVersion()
{
	return version.c_str();;
}

void Application::SetDebugMode(bool set)
{
	debug_mode = set;
}

void Application::GoToBrowser(const char* url)
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}