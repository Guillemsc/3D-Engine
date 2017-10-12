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

	void Focus(vec3 focus, float distance);

	float* GetViewMatrix();

private:
	void CalculateViewMatrix();

	void MoveFront(float speed);
	void MoveBack(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

	void Orbit(vec3 orbit_center, float motion_x, float motion_y);
	void Rotate(float motion_x, float motion_y);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);

	bool IsMouseInsideWindow() const;

public:
	vec3 X, Y, Z, Position, Reference;

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;

	// Camera Movement
	bool  mouse_movement = false;
	float camera_speed = 0.0f;
	float wheel_speed = 0.0f;
	float mouse_sensitivity = 0.0f;
};