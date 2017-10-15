#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "Globals.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "EditorUI.h"
#include "FBO.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/Lib/glew32.lib") 

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	SetName("Renderer3D");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
}

// Called before render is available
bool ModuleRenderer3D::Awake()
{
	bool ret = true;

	LOG_OUTPUT("Creating 3D Renderer context");

	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if (context == NULL)
	{
		LOG_OUTPUT("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Glew
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		LOG_OUTPUT("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}
	else 
	{
		LOG_OUTPUT("Glew library is being used correctly\n");
		LOG_OUTPUT("Using Glew %s\n", glewGetString(GLEW_VERSION));
	}

	// OpenGL
	if (ret == true)
	{
		// get version info
		LOG_OUTPUT("Vendor: %s", glGetString(GL_VENDOR));
		LOG_OUTPUT("Renderer: %s", glGetString(GL_RENDERER));
		LOG_OUTPUT("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG_OUTPUT("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		if (SDL_GL_SetSwapInterval(App->window->GetVsync()) < 0)
			LOG_OUTPUT("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		fbo_texture = new FBO();
		fbo_texture->Create(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_OUTPUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_OUTPUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG_OUTPUT("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		glEnable(GL_DEPTH_TEST);
		lights[0].Active(true);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	SetTexture2D(true);

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate()
{
	bool ret = true;

	fbo_texture->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return ret;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	bool ret = true;

	fbo_texture->Unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Scene

	// Disable light
	SetLightingState(false);

	// Draw editor
	App->editorUI->DrawEditor();

	// Enable light
	SetLightingState(true);

	// Swap
	SDL_GL_SwapWindow(App->window->window);

	return ret;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	bool ret = true;

	LOG_OUTPUT("Destroying 3D Renderer");

	fbo_texture->Unbind();
	RELEASE(fbo_texture);

	SDL_GL_DeleteContext(context);

	return ret;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

void ModuleRenderer3D::SetPoligonModeWireframe() const
{
	bool wireframe = true;
	bool points = false;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ModuleRenderer3D::SetPoligonModePoints(float point_size) const
{
	bool wireframe = false;
	bool points = true;
	bool fill = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPointSize(point_size);
}

void ModuleRenderer3D::SetPoligonModeFill() const
{
	bool wireframe = false;
	bool points = false;
	bool fill = true;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::SetDepthTest(const bool& set) const
{
	if (set)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SetCullFace(const bool& set) const
{
	if (set)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetLightingState(const bool& set) const
{
	if (set)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SetTexture2D(const bool& set) const
{
	if (set)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetColorMaterial(const bool& set) const
{
	if (set)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetAmbientLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, color);

	if (enabled)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}

void ModuleRenderer3D::SetDiffuseLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color);

	if (enabled)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

void ModuleRenderer3D::SetSpecularLight(const bool & enabled, const float color[4]) const
{
	glLightfv(GL_LIGHT2, GL_SPECULAR, color);

	if (enabled)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
}

const bool ModuleRenderer3D::GetPoligonModeWireframe() const
{
	return wireframe;
}

const bool ModuleRenderer3D::GetPoligonModePoints() const
{
	return points;
}

const bool ModuleRenderer3D::GetPoligonModeFill() const
{
	return fill;
}

const bool ModuleRenderer3D::GetDepthTest() const
{
	return glIsEnabled(GL_DEPTH_TEST);
}

const bool ModuleRenderer3D::GetCullFace() const
{
	return glIsEnabled(GL_CULL_FACE);
}

const bool ModuleRenderer3D::GetLightingState() const
{
	return glIsEnabled(GL_LIGHTING);
}

const bool ModuleRenderer3D::GetTexture2D() const
{
	return glIsEnabled(GL_TEXTURE_2D);
}

const bool ModuleRenderer3D::GetColorMaterial() const
{
	return glIsEnabled(GL_COLOR_MATERIAL);
}

uint ModuleRenderer3D::LoadBuffer(float* elements, uint size)
{
	uint id = 0;

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;
}

uint ModuleRenderer3D::LoadBuffer(uint * elements, uint size)
{
	uint id = 0;

	glGenBuffers(1, (GLuint*)&(id));
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * size, elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return id;
}

uint ModuleRenderer3D::LoadTextureBuffer(const void* texture, uint size, int format, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min)
{
	uint id = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(size, (GLuint*)&(id));
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture);

	return id;
}

void ModuleRenderer3D::UnloadBuffer(uint id, uint size)
{
	if(id != 0)
		glDeleteBuffers(1, (GLuint*)&id);
}

void ModuleRenderer3D::UnloadTextureBuffer(uint id, uint size)
{
	glDeleteTextures(size, &id);
}

void ModuleRenderer3D::DrawPlane(float pos_x, float pos_y, float pos_z, int width, int height)
{
	glBegin(GL_QUADS);
	int size = width*height;

	for (int i = 0; i <= size; i++)
	{
		glVertex3f((float)i + pos_x, pos_y, (float)size + pos_z);
		glVertex3f((float)i + pos_x, pos_y, (float)size + pos_z);

		glVertex3f((float)-size + pos_x, pos_y, (float)i + pos_z);
		glVertex3f((float)size + pos_x, pos_y, (float)i + pos_z);
	}
	glEnd();
}

void ModuleRenderer3D::DrawGrid(int HALF_GRID_SIZE)
{
	glBegin(GL_LINES);
	glColor3f(0.75f, 0.75f, 0.75f);
	for (int i = -HALF_GRID_SIZE; i <= HALF_GRID_SIZE; i++)
	{
		glVertex3f((float)i, 0, (float)-HALF_GRID_SIZE);
		glVertex3f((float)i, 0, (float)HALF_GRID_SIZE);

		glVertex3f((float)-HALF_GRID_SIZE, 0, (float)i);
		glVertex3f((float)HALF_GRID_SIZE, 0, (float)i);
	}
	glEnd();

}