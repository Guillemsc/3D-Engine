#include "App.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGameObject.h"
#include "XMLLoader.h"
#include "JSONLoader.h"
#include "EditorUI.h"
#include "Configuration.h"
#include "ModuleFileSystem.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "imgui.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

// TOD0
// I don't like texture saving to dds method

Application::Application(int _argc, char* _args[]) : argc(argc), args(args)
{
	file_system = new FileSystem();
	xml = new XMLLoader();
	json = new JSONLoader();
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	gameobj = new ModuleGameObject();
	editorUI = new EditorUI();
	scene_manager = new SceneManager();
	resource_manager = new ResourceManager();

	// The order of calls is very important!
	// Modules will Awake() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(xml);
	AddModule(json);
	AddModule(file_system);
	AddModule(resource_manager);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(scene_manager);
	AddModule(gameobj);

	AddModule(editorUI);

	// Renderer last
	AddModule(renderer3D);

	profiler = new Profiler();
	id = new IDGen();
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

	profiler->StartProfile(true, "Engine Awake");

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		profiler->StartProfile(true, "Awake %s", (*it)->GetName());

		ret = (*it)->Awake();
		if (!ret) return false;

		profiler->FinishProfile("Awake %s", (*it)->GetName());
	}

	LoadConfig();

	profiler->FinishProfile("Engine Awake");

	return ret;
}

bool Application::Start()
{
	bool ret = true;

	profiler->StartProfile(true, "Engine Start");

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		profiler->StartProfile(true, "Start %s", (*it)->GetName());

		ret = (*it)->Start();
		if (!ret) return false;

		profiler->FinishProfile("Start %s", (*it)->GetName());
	}

	profiler->FinishProfile("Engine Start");

	profiler->SetEnabled(false);

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	profiler->UpdateStart();
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;

	if (input->GetWindowEvent(WE_QUIT) == true || end_app)
		return false;

	PrepareUpdate();

	profiler->StartProfile(false, "Engine Update");

	// PreUpdate
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		ret = (*it)->PreUpdate();

		if (!ret) return false;
	}

	// Update
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->GetEnabled())
			continue;

		profiler->StartProfile(false, "Update %s", (*it)->GetName());

		ret = (*it)->Update();

		profiler->FinishProfile("Update %s", (*it)->GetName());

		if (!ret) return false;
	}

	profiler->FinishProfile("Engine Update");

	// PostUpdate
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
	profiler->UpdateFinish();
}

bool Application::CleanUp()
{
	bool ret = true;

	SaveConfig();

	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
	{
		ret = (*it)->CleanUp();

		if (!ret) return false;
	}

	delete id;

	profiler->CleanUp();
	delete profiler;

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
		config->MoveToRoot();

		const char* title = config->GetString("app.title", "No title");
		const char* organization = config->GetString("app.organization", "No org");
		const char* version = config->GetString("app.version", "No version");
		int max_fps = config->GetNumber("app.max_fps", 60);

		SetAppName(title);
		SetAppOrganization(organization);
		SetVersion(version);
		SetMaxFps(max_fps);

		for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
		{
			(*it)->OnLoadConfig(config);
		}
	}
}

void Application::SaveConfig(Module* module)
{
	if (config != nullptr)
	{
		config->MoveToRoot();

		config->SetString("app.title", App->GetAppName());
		config->SetString("app.organization", App->GetAppOrganization());
		config->SetNumber("app.max_fps", App->GetMaxFps());
		config->SetString("app.version", App->GetVersion());

		for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++)
		{
			if(module == nullptr)
				(*it)->OnSaveConfig(config);

			else if(module == (*it))
				(*it)->OnSaveConfig(config);	
		}

		config->Save();
	}
}

JSON_Doc* Application::GetConfig() const
{
	return config;
}

void Application::LoadFile(const char * filepath)
{
	string name = App->file_system->GetFileNameFromFilePath(filepath);
	string extension = ToLowerCase(App->file_system->GetFileExtension(name.c_str()));

	for (list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->OnLoadFile(filepath, name.c_str(), extension.c_str());
	}
}



void Application::EndApp()
{
	end_app = true;
}

float Application::GetDT() const
{
	return profiler->GetFrameTime()/1000;
}

void Application::SetAppName(const char* name)
{
	if (title != name)
	{
		title = name;
		window->SetTitle(name);
	}
}

const char * Application::GetAppName() const
{
	return title.c_str();
}

void Application::SetAppOrganization(const char* name)
{
	if (name != organization)
	{
		organization = name;
	}
}

const char * Application::GetAppOrganization()
{
	return organization.c_str();
}

void Application::SetMaxFps(int const &set)
{
	profiler->SetMaxFps(set);
}

int Application::GetMaxFps() const
{
	return profiler->GetMaxFps();
}

bool Application::GetDebugMode() const
{
	return debug_mode;
}

void Application::SetVersion(const char * set)
{
	version = set;
}

const char * Application::GetVersion() const
{
	return version.c_str();;
}

SDL_version Application::GetSDLVersion()
{
	SDL_version version;
	SDL_GetVersion(&version);
	return version;
}

const char * Application::GetBasePath()
{
	return SDL_GetBasePath();
}

void Application::SetDebugMode(bool set)
{
	debug_mode = set;
}

void Application::GoToBrowser(const char* url)
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::GoToFolder(const char * folder)
{
	ShellExecute(NULL, "open", folder, NULL, NULL, SW_SHOWMAXIMIZED);
}

void Application::AddModule(Module* mod)
{
	modules.push_back(mod);
}