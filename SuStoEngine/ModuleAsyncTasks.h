#ifndef _MODULE_ASYNC_TASKS_H_
#define _MODULE_ASYNC_TASKS_H_

#include "Module.h"

class ModuleAsyncTasks;

enum AsyncTaskMode
{
	AST_FOCUS,
	AST_BACKGROUND,
};

class AsyncTask
{
	friend ModuleAsyncTasks;

public:
	AsyncTask(AsyncTaskMode mode, uint iterations_per_frame);

	virtual void Start() {};
	virtual void Update() {};
	virtual void Finish() {};

	void FinishTask();
	AsyncTaskMode GetMode();

private:
	AsyncTaskMode  mode = AsyncTaskMode::AST_FOCUS;
	bool		   finished = false;
	uint		   iterations_per_frame = 0;
};

class ModuleAsyncTasks : public Module
{
public:
	ModuleAsyncTasks(bool start_enabled = true);
	~ModuleAsyncTasks();

	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void StartTask(AsyncTask* start);

private:
	std::vector<AsyncTask*> running_tasks;
};

#endif //!_MODULE_ASYNC_TASKS_H_