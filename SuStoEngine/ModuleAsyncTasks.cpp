#include "ModuleAsyncTasks.h"
#include "App.h"
#include "ModuleEventSystem.h"
#include "imgui.h"

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

	for (std::list<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end();)
	{
		AsyncTask* curr = (*it);

		if (!curr->finished)
		{
			for (int i = 0; i < (*it)->iterations_per_frame; ++i)
			{
				if (curr->first_update)
				{
					AsyncTaskModeStart((*it));

					if (curr->on_start)
						curr->on_start(*it);

					curr->first_update = false;
				}

				if (curr->on_update)
					curr->on_update(*it);

				curr->Update();
			}

			++it;
		}
		else
		{
			curr->Finish();

			AsyncTaskModeFinish(curr);

			if (curr->on_finish)
				curr->on_finish(curr);

			Event ev(EventType::ET_ASYNC_TASK_FINISHED);
			ev.async_task_finished.task = curr;
			App->event_system->Send(ev);

			it = running_tasks.erase(it);

			delete curr;
		}
	}

	return ret;
}

bool ModuleAsyncTasks::Update()
{
	bool ret = true;

	for (std::list<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end(); ++it)
	{
		uint flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize 
			| ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings;

		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetNextWindowPos(ImVec2((size.x * 0.5), (size.y * 0.5f - 60)));
		if (ImGui::Begin((*it)->task_name.c_str(), nullptr, flags))
		{
			ImGui::Text((*it)->phase.c_str());
			ImGui::ProgressBar((*it)->progress * 0.01f, ImVec2(400, 0));
			ImGui::End();
		}
	}

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

	for (std::list<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end();)
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

void ModuleAsyncTasks::AsyncTaskModeStart(AsyncTask* task)
{
	if (task != nullptr)
	{
		switch (task->GetMode())
		{
		case AsyncTaskMode::AST_BACKGROUND:
			break;

		case AsyncTaskMode::AST_FOCUS:
			App->editorUI->SetEditorInteractable(false);
			break;
		}
	}
}

void ModuleAsyncTasks::AsyncTaskModeFinish(AsyncTask* task)
{
	if (task != nullptr)
	{
		switch (task->GetMode())
		{
		case AsyncTaskMode::AST_BACKGROUND:
			break;

		case AsyncTaskMode::AST_FOCUS:
			bool can_disable = true;

			for (std::list<AsyncTask*>::iterator it = running_tasks.begin(); it != running_tasks.end(); ++it)
			{
				if ((*it)->GetMode() == AsyncTaskMode::AST_FOCUS && (*it) != task)
				{
					can_disable = false;
					break;
				}

				if (can_disable)
					App->editorUI->SetEditorInteractable(true);
			}

			break;
		}
	}
}

AsyncTask::AsyncTask(AsyncTaskMode _mode, uint _iterations_per_frame, std::string _task_name)
{
	mode = _mode;
	iterations_per_frame = _iterations_per_frame;
	task_name = _task_name;
}

void AsyncTask::FinishTask()
{
	finished = true;
}

void AsyncTask::SetPercentageProgress(float set)
{
	progress = set;

	if (progress < 0)
		progress = 0.0f;

	if (progress > 100)
		progress = 100.0f;
}

float AsyncTask::GetPercentageProgress()
{
	return progress;
}

void AsyncTask::SetCurrPhase(std::string _phase)
{
	phase = _phase;
}

std::string AsyncTask::GetCurrPhase()
{
	return phase;
}

AsyncTaskMode AsyncTask::GetMode()
{
	return mode;
}

void AsyncTask::OnStart(std::function<void(AsyncTask*)> fun)
{
	on_start = fun;
}

void AsyncTask::OnUpdate(std::function<void(AsyncTask*)> fun)
{
	on_update = fun;
}

void AsyncTask::OnFinish(std::function<void(AsyncTask*)> fun)
{
	on_finish = fun;
}
