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

	void Follow(PhysBody3D* body, float min, float max, float height, float displacement);

private:

	void CalculateViewMatrix();

public:

	vec3 X, Y, Z, Position, Reference;

	float camera_distance			= 0;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;

	// Camera follow
	float min_following_dist = 0;
	float max_following_dist = 0;
	float following_height = 0;
	float displacement = 0;
};