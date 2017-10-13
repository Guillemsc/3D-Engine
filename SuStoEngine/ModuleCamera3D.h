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

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	void SetMouseSensitivity(const float& set);
	void SetWheelSpeed(const float& set);
	void SetCameraSpeed(const float& set);

	const float GetMouseSensitivity() const;
	const float GetWheelSpeed() const;
	const float GetCameraSpeed() const;

	void Focus(const vec3& focus, const float& distance);

	const float* GetViewMatrix() const;

private:
	void CalculateViewMatrix();

	void MoveFront(const float& speed);
	void MoveBack(const float& speed);
	void MoveLeft(const float& speed);
	void MoveRight(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	void Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);

	void Look(const vec3& Position, const vec3& Reference, bool RotateAroundReference = false);
	void LookAt(const vec3& Spot);
	void Move(const vec3& Movement);

	const bool IsMouseInsideWindow() const;

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