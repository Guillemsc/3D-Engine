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

	CalculateViewMatrix();

	// Normalized vectors that define the direction of X, Y and Z
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	// Position of the camera X, Y and Z
	Position = vec3(0.0f, 0.0f, 0.0f);

	// Reference point in which is rotating from
	Reference = vec3(0.0f, 0.0f, 0.0f);

	Focus(vec3(0.0f, 0.0f, 0.0f), 10);
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

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;

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
			MoveFront(wheel_speed);
		}
		else if (App->input->GetMouseWheel() == -1)
		{
			MoveBack(wheel_speed);
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			App->window->GetCursor()->Hand();

			if (App->input->GetKeyRepeat(SDL_SCANCODE_Z))
				MoveUp(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_X))
				MoveDown(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
				MoveFront(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_S))
				MoveBack(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_A))
				MoveLeft(speed);

			if (App->input->GetKeyRepeat(SDL_SCANCODE_D))
				MoveRight(speed);

			else
			{
				Rotate(App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			}
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
			{
				Orbit(vec3(0, 0, 0), App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

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
		Focus(vec3(0, 0, 0), 10);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return ret;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
const float* ModuleCamera3D::GetViewMatrix() const
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::MoveFront(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos -= Z * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveBack(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos += Z * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveLeft(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos -= X * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveRight(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos += X * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveUp(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos.y += speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveDown(const float& speed)
{
	vec3 newPos(0, 0, 0);
	newPos.y -= speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::Focus(const vec3& focus, const float& distance)
{
	Reference = focus;

	Position = Reference + Z * distance;
}

void ModuleCamera3D::Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y)
{
	Reference = orbit_center;

	int dx = -motion_x;
	int dy = -motion_y;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * mouse_sensitivity;
		
		// Rotate arround the y axis
		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * mouse_sensitivity;

		// Rotate arround the X direction
		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);
	}

	Position = Reference + Z * length(Position);
}

void ModuleCamera3D::Rotate(const float& motion_x, const float& motion_y)
{
	Reference = Position;

	int dx = -motion_x;
	int dy = -motion_y;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * mouse_sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * mouse_sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);
	}

	Position = Reference + Z * length(Position);
}

const bool ModuleCamera3D::IsMouseInsideWindow() const
{
	return PointInRect(float2(App->input->GetMouseX(), App->input->GetMouseY()), App->editorUI->GameRect());
}
