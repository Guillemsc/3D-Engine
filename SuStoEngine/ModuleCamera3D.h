#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:
	void CalculateViewMatrix();

public:
	vec3 X, Y, Z, Position, Reference;

	float camera_distance = 0;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	// Camera Movement
	bool is_inside_rect = false;
};