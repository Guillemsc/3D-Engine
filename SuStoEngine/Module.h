#pragma once

class Application;
struct PhysBody3D;

using namespace std;

#include <string>
#include <list>
#include <queue>
#include <iostream>

#include "SDL\include\SDL.h"
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

	void SetName(const char* set_name) { name = set_name; }
	const char* GetName() { return name.c_str(); };
	bool GetEnabled() { return enabled; };
	void SetEnabled(bool set) { enabled = set; };

private:
	string name = "";
	bool   enabled = false;
};