#pragma once

#include "Module.h"
#include "Globals.h"
#include "GeometryMath.h"
#include "glmath.h"

class Camera3D
{
public:
	Camera3D();

	void SetPosition(const float3& pos);
	const float3 GetPosition();

	void GetCorners(float3* corners);

	void SetNearPlaneDistance(const float& set);
	void SetFarPlaneDistance(const float& set);
	void SetFOV(const float& set);
	void SetAspectRatio(const float& set);
	const float GetNearPlaneFistance() const;
	const float GetFarPlaneDistance() const;
	const float GetVerticalFOV() const;
	const float GetHorizontalFOV() const;
	const float4x4 GetViewMatrix() const;
	const float4x4 GetProjectionMatrix() const;
	const float* GetOpenGLViewMatrix() const;
	const float* GetOpenGLProjectionMatrix() const;

	void MoveFront(const float& speed);
	void MoveBack(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	void Orbit(const float3& rotate_center, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void Focus(const float3& focus_center, const float& distance);

	void Look(const float3& look_pos);

private:
	Frustum frustum;
	float	aspect_ratio = 0.0f;
};

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

	Camera3D* GetEditorCamera() const;

	void SetMouseSensitivity(const float& set);
	void SetWheelSpeed(const float& set);
	void SetCameraSpeed(const float& set);

	const float GetMouseSensitivity() const;
	const float GetWheelSpeed() const;
	const float GetCameraSpeed() const;

	void SetCurrentCamera(Camera3D* set);
	Camera3D* GetCurrentCamera() const;

	const float* GetViewMatrix() const;

private:
	const bool IsMouseInsideWindow() const;

private:
	Camera3D* editor_camera = nullptr;
	Camera3D* current_camera = nullptr;

	// Camera Movement
	bool  mouse_movement = false;
	float camera_speed = 0.0f;
	float wheel_speed = 0.0f;
	float mouse_sensitivity = 0.0f;
};