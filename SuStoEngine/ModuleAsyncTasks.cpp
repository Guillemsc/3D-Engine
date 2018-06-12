#include "ModuleAsyncTasks.h"
#include "App.h"
#include "ModuleEventSystem.h"

ModuleAsyncTasks::ModuleAsyncTasks(bool start_enabled) : Module(start_enabled)
{
	SetName("AsyncTasks");
}

ModuleAsyncTasks::~ModuleAsyncTasks()
{
}

bool ModuleAsyncTasks::PreUpdate()
{
	bool ret = true;

	for (std::vector<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end();)
	{
		if (!(*it)->finished)
		{
			for(int i = 0; i < (*it)->iterations_per_frame; ++i)
				(*it)->Update();

			++it;
		}
		else
		{
			(*it)->Finish();

			Event ev(EventType::ET_ASYNC_TASK_FINISHED);
			ev.async_task_finished.task = (*it);
			App->event_system->Send(ev);

			delete (*it);
			it = running_tasks.erase(it);
		}
	}

	return ret;
}

bool ModuleAsyncTasks::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleAsyncTasks::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleAsyncTasks::CleanUp()
{
	bool ret = true;

	for (std::vector<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end();)
	{
		delete (*it);
		it = running_tasks.erase(it);
	}

	return ret;
}

void ModuleAsyncTasks::StartTask(AsyncTask* start)
{
	if (start != nullptr)
	{
		running_tasks.push_back(start);

		Event ev(EventType::ET_ASYNC_TASK_STARTED);
		ev.async_task_finished.task = start;
		App->event_system->Send(ev);

		start->Start();
	}
}

AsyncTask::AsyncTask(AsyncTaskMode _mode, uint _iterations_per_frame)
{
	mode = _mode;
	iterations_per_frame = _iterations_per_frame;
}

void AsyncTask::FinishTask()
{
	finished = true;
}

AsyncTaskMode AsyncTask::GetMode()
{
	return mode;
}
