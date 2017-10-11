#include "Globals.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleCamera3D.h"
#include "EditorUI.h"
#include "Game.h"
#include "imgui.h"

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

	camera_speed = 20.0f;
	wheel_speed = 5.0f;
	mouse_sensitivity = 0.25f;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG_OUTPUT("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	bool ret = true;

	float speed = camera_speed * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT))
		speed = camera_speed/2 * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_R))
		MoveUp(speed);

	if (App->input->GetKeyRepeat(SDL_SCANCODE_F))
		MoveDown(speed);

	if (App->input->GetKeyRepeat(SDL_SCANCODE_W))
		MoveFront(speed);

	if (App->input->GetKeyRepeat(SDL_SCANCODE_S)) 
		MoveBack(speed);

	if (App->input->GetKeyRepeat(SDL_SCANCODE_A))
		MoveLeft(speed);

	if (App->input->GetKeyRepeat(SDL_SCANCODE_D))
		MoveRight(speed);


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
			if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
			{
				Orbit(vec3(0, 0, 0), App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			}
			else
			{
				Rotate(App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			}
		}
		else
		{
			mouse_movement = false;
		}
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
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::MoveFront(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos -= Z * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveBack(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos += Z * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveLeft(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos -= X * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveRight(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos += X * speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveUp(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos.y += speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::MoveDown(float speed)
{
	vec3 newPos(0, 0, 0);
	newPos.y -= speed;
	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::Orbit(vec3 orbit_center, float motion_x, float motion_y)
{
	LookAt(vec3(0, 0, 0));
	orbit_center;

	int dx = -motion_x;
	int dy = -motion_y;

	Position -= orbit_center;

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

		if (Y.y < 0.0f)
		{
			//Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			//Y = cross(Z, X);
		}
	}

	Position = orbit_center + Z * length(Position);
}

void ModuleCamera3D::Rotate(float motion_x, float motion_y)
{
	int dx = -motion_x;
	int dy = -motion_y;

	vec3 ref = Position;

	Position -= ref;

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

	Position = ref + Z * length(Position);
}

bool ModuleCamera3D::IsMouseInsideWindow() const
{
	return PointInRect(float2(App->input->GetMouseX(), App->input->GetMouseY()), App->editorUI->GameRect());
}
