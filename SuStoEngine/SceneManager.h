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

struct Relation
{
	Relation(int _id, GameObject* _go)
	{
		id = _id;
		go = _go;
		id_parent = -1;
	}
	Relation(int _id, GameObject* _go, int _id_parent)
	{
		id = _id;
		go = _go;
		id_parent = _id_parent;
	}

	int id = 0;
	GameObject* go = nullptr;
	int id_parent = 0;
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

	bool SavePrefab(const char* name, const char* extension, const char* path, GameObject* go);
	bool LoadPrefab(const char* file_path, GameObject *& loaded_go);
	bool LoadPrefab(const char* file_path);

	SceneState GetState();
	bool GetPause();
	bool GetStep();

	void Edit();
	void Play();
	void Pause();
	void Step();

	float GetGameDT();
	float GetGameExecutionTime();

private:
	void ClearRelations();
	void AddRelationGo(GameObject* go);
	void AddRleationIdGo(int id, GameObject* go, int parent_id = -1);
	int GetRelationGo(GameObject* go);
	GameObject* GetRelationId(int id);

public:

private:
	SceneState state = EDIT;
	bool	   pause = false;
	bool	   step = false;
	string	   current_scene;

	std::vector<Relation> relations;
};

#endif