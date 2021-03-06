#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;

enum SceneState
{
	EDIT,
	PLAY,
};

class SceneManager : public Module
{
public:
	SceneManager(bool start_enabled = true);
	virtual ~SceneManager();

	bool Start();
	bool CleanUp();

	void SaveScene(const char* scene_name);
	void LoadScene(const char* scene_name, bool set_scene_title = true);
	void DestroyScene();

	void SaveTmpScene();
	void LoadTmpScene();

	SceneState GetState();
	bool GetPause();
	bool GetStep();

	void Edit();
	void Play();
	void Pause();
	void Step();

	float GetGameDT();
	float GetGameExecutionTime();

public:

private:
	SceneState state = EDIT;
	bool	   pause = false;
	bool	   step = false;
	string	   current_scene;
};

#endif