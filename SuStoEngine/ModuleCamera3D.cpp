#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "EditorUI.h"
#include "Game.h"
#include "ModuleWindow.h"
#include "Cursor.h"
#include "imgui.h"
#include "JSONLoader.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	SetName("Camera3D");

	editor_camera = new Camera3D();
	current_camera = editor_camera;
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Loading Camera3D");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	bool ret = true;

	LOG_OUTPUT("Setting up the camera");

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Cleaning camera");

	delete editor_camera;

	return ret;
}

void ModuleCamera3D::OnLoadConfig(JSON_Doc * config)
{
	mouse_sensitivity = config->GetNumber("camera.mouse_sensitivity", 0.25f);
	wheel_speed = config->GetNumber("camera.wheel_speed", 5.0f);
	camera_speed = config->GetNumber("camera.camera_speed", 20.0f);
}

void ModuleCamera3D::OnSaveConfig(JSON_Doc * config)
{
	config->SetNumber("camera.mouse_sensitivity", mouse_sensitivity);
	config->SetNumber("camera.wheel_speed", wheel_speed);
	config->SetNumber("camera.camera_speed", camera_speed);
}

Camera3D * ModuleCamera3D::GetEditorCamera() const
{
	return editor_camera;
}

void ModuleCamera3D::SetMouseSensitivity(const float& set)
{
	mouse_sensitivity = set;
}

void ModuleCamera3D::SetWheelSpeed(const float& set)
{
	wheel_speed = set;
}

void ModuleCamera3D::SetCameraSpeed(const float& set)
{
	camera_speed = set;
}

const float ModuleCamera3D::GetMouseSensitivity() const
{
	return mouse_sensitivity;
}

const float ModuleCamera3D::GetWheelSpeed() const
{
	return wheel_speed;
}

const float ModuleCamera3D::GetCameraSpeed() const
{
	return camera_speed;
}

void ModuleCamera3D::SetCurrentCamera(Camera3D * set)
{
	if (set != nullptr)
		current_camera = set;
}

Camera3D * ModuleCamera3D::GetCurrentCamera() const
{
	return current_camera;
}

const float * ModuleCamera3D::GetViewMatrix() const
{
	return current_camera->GetViewMatrix().Transposed().ptr();
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;

	if (editor_camera == nullptr)
		return true;

	float speed = camera_speed * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT))
		speed = camera_speed/2 * App->GetDT();

	if (IsMouseInsideWindow())
	{
		mouse_movement = true;
	}

	// Mouse motion ----------------
	if (mouse_movement)
	{
		if (App->input->GetMouseWheel() == 1)
		{
			editor_camera->MoveFront(wheel_speed);
		}
		else if (App->input->GetMouseWheel() == -1)
		{
			editor_camera->MoveBack(wheel_speed);
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			App->window->GetCursor()->Hand();

			if (App->input->GetKeyRepeat(SDL_SCANCODE_Z))
				editor_camera->MoveUp(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_X))
				editor_camera->MoveDown(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
				editor_camera->MoveFront(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
				editor_camera->MoveBack(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_A))
				editor_camera->MoveLeft(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_D))
				editor_camera->MoveRight(speed);

			editor_camera->Rotate(-App->input->GetMouseXMotion()*mouse_sensitivity*0.01f, -App->input->GetMouseYMotion()*mouse_sensitivity*0.01f);
			
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
			{
				if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
					editor_camera->MoveFront(speed);

				if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
					editor_camera->MoveBack(speed);

				editor_camera->Orbit(float3(0, 0, 0), -App->input->GetMouseXMotion()*mouse_sensitivity*0.01f, -App->input->GetMouseYMotion()*mouse_sensitivity*0.01f);
				editor_camera->Look(float3(0, 0, 0));

				App->window->GetCursor()->SizeAll();
			}
		}
		else
		{
			mouse_movement = false;
		}
	}

	if (App->input->GetKeyDown("f"))
	{
		editor_camera->Focus(float3(0, 0, 0), 10);
	}

	return ret;
}

const bool ModuleCamera3D::IsMouseInsideWindow() const
{
	return PointInRect(float2(App->input->GetMouseX(), App->input->GetMouseY()), App->editorUI->GameRect());
}

Camera3D::Camera3D()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 0, -1);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 0;
	frustum.horizontalFov = 0;
	frustum.farPlaneDistance = 1.0f;
	frustum.nearPlaneDistance = 0;
	aspect_ratio = 0;

	SetNearPlaneDistance(0.1f);
	SetFarPlaneDistance(500.0f);
	SetAspectRatio(1.3f);
	SetFOV(60);
}

void Camera3D::SetPosition(const float3 & pos)
{
	frustum.pos = pos;
}

const float3 Camera3D::GetPosition()
{
	return frustum.pos;
}

void Camera3D::GetCorners(float3* corners)
{
	frustum.GetCornerPoints(corners);
}

void Camera3D::SetNearPlaneDistance(const float & set)
{
	if (set > 0 && set < frustum.farPlaneDistance)
		frustum.nearPlaneDistance = set;
}

void Camera3D::SetFarPlaneDistance(const float & set)
{
	if (set > 0 && set > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = set;
}

void Camera3D::SetFOV(const float & set)
{
	if(set > 0)
		frustum.verticalFov = DEGTORAD * set;

	if (aspect_ratio > 0)
		frustum.horizontalFov = frustum.verticalFov * aspect_ratio;
	/*	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);*/ //  wtf is this?

	float aspect_ration = frustum.AspectRatio();
}

void Camera3D::SetAspectRatio(const float & set)
{
	aspect_ratio = set;

	if(frustum.verticalFov > 0)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
}

const float Camera3D::GetNearPlaneFistance() const
{
	return frustum.nearPlaneDistance;
}

const float Camera3D::GetFarPlaneDistance() const
{
	return frustum.farPlaneDistance;
}

const float Camera3D::GetVerticalFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

const float Camera3D::GetHorizontalFOV() const
{
	return frustum.horizontalFov;
}

const float4x4 Camera3D::GetViewMatrix() const
{
	return frustum.ViewMatrix();
}

const float4x4 Camera3D::GetProjectionMatrix() const
{
	return frustum.ProjectionMatrix();
}

const float * Camera3D::GetOpenGLViewMatrix() const
{
	static float4x4 view = frustum.ViewMatrix();
	view.Transpose();
	return view.ptr();
}

const float * Camera3D::GetOpenGLProjectionMatrix() const
{
	return frustum.ProjectionMatrix().Transposed().ptr();
}

void Camera3D::MoveFront(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveBack(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.front * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveRight(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveLeft(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= frustum.WorldRight() * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveUp(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement += float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::MoveDown(const float & speed)
{
	if (speed <= 0)
		return;

	float3 movement = float3::zero;
	movement -= float3::unitY * speed;
	frustum.Translate(movement);
}

void Camera3D::Orbit(const float3 & rotate_center, const float & motion_x, const float & motion_y)
{
	float3 distance = frustum.pos - rotate_center;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.up, motion_x);

	distance = X.Transform(distance);
	distance = Y.Transform(distance);

	frustum.pos = distance + rotate_center;
}

void Camera3D::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.front = rotation_x.Mul(frustum.front).Normalized();
	frustum.up = rotation_x.Mul(frustum.up).Normalized();

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.front = rotation_y.Mul(frustum.front).Normalized();
	frustum.up = rotation_y.Mul(frustum.up).Normalized();
}

void Camera3D::Look(const float3 & look_pos)
{
	float3 dir = look_pos - frustum.pos;

	float3x3 direction_matrix = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = direction_matrix.MulDir(frustum.front).Normalized();
	frustum.up = direction_matrix.MulDir(frustum.up).Normalized();
}

void Camera3D::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.pos - focus_center;
	frustum.pos = dir.Normalized() * distance;

	Look(focus_center);
}
