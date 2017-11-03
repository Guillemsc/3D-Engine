#pragma once

class Application;
struct PhysBody3D;
class JSON_Doc;

using namespace std;

#include <string>
#include <list>
#include <queue>
#include <iostream>

#include "SDL/include/SDL.h"
#include "Globals.h"

class Module
{
public:
	Module(bool start_enabled = true) : enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void OnLoadConfig(JSON_Doc* config) {};
	virtual void OnSaveConfig(JSON_Doc* config) {};

	virtual void LoadScene(JSON_Doc* config) {};
	virtual void SaveScene(JSON_Doc* config) {};

	virtual void OnLoadFile(const char* file_path, const char* file_name, const char* file_extension) {};

	void SetName(const char* set_name) { name = set_name; }
	const char* GetName() { return name.c_str(); };
	bool GetEnabled() { return enabled; };
	void SetEnabled(bool set) { enabled = set; };

private:
	string name = "";
	bool   enabled = false;
};