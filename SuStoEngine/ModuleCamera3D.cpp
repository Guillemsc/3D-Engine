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

	camera_distance = 0;
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
	LOG_OUTPUT("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update()
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	//(following != nullptr && !App->circuits->choose_level)
	//(following != nullptr && App->physics->debug == true)

	bool ret = true;

	vec3 newPos(0, 0, 0);
	float speed = 20.0f * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT))
		speed = 8.0f * App->GetDT();

	if (App->input->GetKeyRepeat(SDL_SCANCODE_R)) newPos.y += speed;
	if (App->input->GetKeyRepeat(SDL_SCANCODE_F)) newPos.y -= speed;

	if (App->input->GetKeyRepeat(SDL_SCANCODE_W)) newPos -= Z * speed;
	if (App->input->GetKeyRepeat(SDL_SCANCODE_S)) newPos += Z * speed;

	if (App->input->GetKeyRepeat(SDL_SCANCODE_A)) newPos -= X * speed;
	if (App->input->GetKeyRepeat(SDL_SCANCODE_D)) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------
	
	is_inside_rect = PointInRect(float2(App->input->GetMouseX(), App->input->GetMouseY()), App->editorUI->GameRect());
	
	if (is_inside_rect && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT
		&& (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT)))
	{

	}

	if (is_inside_rect && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKeyRepeat(SDL_SCANCODE_LALT) || App->input->GetKeyRepeat(SDL_SCANCODE_RALT))
		{
			LookAt(vec3(0, 0, 0));
			Reference = vec3(0, 0, 0);
		}
		else
		{
			Reference = Position;
		}

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				//Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				//Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
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