#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "Globals.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "EditorUI.h"
#include "FBO.h"
#include "SDL/include/SDL_opengl.h"
#include "ImGuizmo.h"
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

	CONSOLE_LOG("Creating 3D Renderer context");

	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	if (context == NULL)
	{
		CONSOLE_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Glew
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		CONSOLE_LOG("Glew library could not init %s\n", glewGetErrorString(err));
		ret = false;
	}
	else 
	{
		CONSOLE_LOG("Glew library is being used correctly\n");
		CONSOLE_LOG("Using Glew %s\n", glewGetString(GLEW_VERSION));
	}

	// OpenGL
	if (ret == true)
	{
		// get version info
		CONSOLE_LOG("Vendor: %s", glGetString(GL_VENDOR));
		CONSOLE_LOG("Renderer: %s", glGetString(GL_RENDERER));
		CONSOLE_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		CONSOLE_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		if (SDL_GL_SetSwapInterval(App->window->GetVsync()) < 0)
			CONSOLE_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		fbo_texture = new FBO();
		fbo_texture->Create(App->window->GetWindowSize().x, App->window->GetWindowSize().y);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
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
	float light[4] = { 255, 255, 255, 255 };
	SetAmbientLight(true, light);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate()
{
	bool ret = true;

	//fbo_texture->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->GetCurrentCamera()->GetOpenGLProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetCurrentCamera()->GetOpenGLViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(0, 0, 0);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return ret;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	bool ret = true;

	// Finish 3d Draw scene
	//fbo_texture->Unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	CONSOLE_LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return ret;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	App->camera->GetCurrentCamera()->SetAspectRatio((float)width / (float)height);
}

void ModuleRenderer3D::RenderScene()
{
	std::vector<Camera3D*> cameras = App->camera->GetCameras();

	float2 window_size = App->window->GetWindowSize();

	for (std::vector<Camera3D*>::iterator it = cameras.begin(); it != cameras.end(); ++it)
	{
		Camera3D* curr_camera = (*it);

		curr_camera->Bind(0, 0, window_size.x, window_size.y);

		curr_camera->Unbind();
	}
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

void ModuleRenderer3D::SetViewport(uint start_x, uint start_y, uint width, uint height)
{
	glViewport(start_x, start_y, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error setting viewport: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::GetViewport(uint & start_x, uint & start_y, uint & width, uint & height)
{
	GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);

	start_x = viewport[0];
	start_y = viewport[1];
	width = viewport[2];
	height = viewport[3];

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error getting viewport: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::Clear(uint buffer)
{
	glClear(buffer);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error clearing buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenBuffer() const
{
	uint ret = 0;

	glGenBuffers(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::BindArrayBuffer(uint id) const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error bind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::BindElementArrayBuffer(uint id) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error bind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderElement(uint num_indices) const
{
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error draw elements: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindArraybuffer() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindElementArrayBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::EnableState(GLenum type) const
{
	glEnableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error enable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DisableState(GLenum type) const
{
	glDisableClientState(type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error disable state: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetVertexPointer() const
{
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error set vertex pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetNormalsPointer() const
{
	glNormalPointer(GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error set normals pointer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetTexCoordPointer()
{
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error set texcoord pointer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenTexture() const
{
	uint ret = 0;

	glGenTextures(1, &ret);

	return ret;
}

void ModuleRenderer3D::BindTexture(uint id) const
{
	glBindTexture(GL_TEXTURE_2D, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::BindTexture(uint target, uint id) const
{
	glBindTexture((GLenum)target, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error bind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindTexture(uint target) const
{
	glBindTexture((GLenum)target, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DeleteTexture(uint& id) const
{
	if (id > 0)
	{
		glDeleteTextures(1, &id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error deleting texture: %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::GenRenderBuffer() const
{
	uint ret = 0;

	glGenRenderbuffers(1, &ret);

	return ret;
}

void ModuleRenderer3D::BindRenderBuffer(uint id) const
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error binding render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindRenderBuffer() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::Set2DMultisample(uint samples, uint width, uint height)
{
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error setting 2D multisample texture: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetFrameBufferTexture2D(uint id)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, id, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error setting frame buffer texture2D: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderStorageMultisample(uint samples, uint width, uint height)
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error rendering storage multisample: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::RenderRenderBuffer(uint samples, uint width, uint height)
{
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error rendering render buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::LoadArrayToVRAM(uint size, float * values, GLenum type) const
{
	glBufferData(GL_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::LoadArrayToVRAM(uint size, uint * values, GLenum type) const
{
	glBufferData(GL_ARRAY_BUFFER, size, values, type);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error load array to vram: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::LoadTextureToVRAM(uint w, uint h, GLubyte * tex_data, GLint format) const
{
	uint buff_id = 0;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error load texture to vram e1: %s\n", gluErrorString(error));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &buff_id);
	glBindTexture(GL_TEXTURE_2D, buff_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, tex_data);

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error load texture to vram e2: %s\n", gluErrorString(error));
	}

	return buff_id; uint();
}

void ModuleRenderer3D::PushMatrix()
{
	glPushMatrix();
}

void ModuleRenderer3D::PopMatrix()
{
	glPopMatrix();
}

void ModuleRenderer3D::MultMatrix(float * matrix)
{
	glMultMatrixf(matrix);
}

uint ModuleRenderer3D::GenVertexArrayBuffer() const
{
	uint ret = 0;

	glGenVertexArrays(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::BindVertexArrayBuffer(uint id) const
{
	glBindVertexArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error bind vertex array buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindVertexArrayBuffer() const
{
	glBindVertexArray(0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbind array buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::GenFrameBuffer() const
{
	uint ret = 0;

	glGenFramebuffers(1, (GLuint*)&ret);

	return ret;
}

void ModuleRenderer3D::BindFrameBuffer(uint id) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error binding frame buffer: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error unbinding frame buffer: %s\n", gluErrorString(error));
	}
}

uint ModuleRenderer3D::CheckFrameBufferStatus()
{
	uint ret = 0;

	ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error checking frame buffer status: %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::DeleteFrameBuffer(uint & id)
{
	if (id > 0)
	{
		glDeleteFramebuffers(1, &id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error deleting frame buffer: %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::CreateVertexShader(const char * source)
{
	GLuint vertexShader = 0;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (success == 0)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		CONSOLE_LOG("Shader compilation error:\n %s", infoLog);
		glDeleteShader(vertexShader);

		vertexShader = 0;
	}

	return vertexShader;
}

uint ModuleRenderer3D::CreateFragmentShader(const char * source)
{
	GLuint fragmentShader = 0;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (success == 0)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		CONSOLE_LOG("Shader compilation error:\n %s", infoLog);
		glDeleteShader(fragmentShader);

		fragmentShader = 0;
	}

	return fragmentShader;
}

void ModuleRenderer3D::DeleteShader(uint shader_id)
{
	if (shader_id > 0)
	{
		glDeleteShader(shader_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error deleting shader %s\n", gluErrorString(error));
		}
	}
}

uint ModuleRenderer3D::GetProgramBinary(uint program_id, uint buff_size, char * buff) const
{
	uint ret = 0;

	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	GLint *binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	glGetProgramBinary(program_id, buff_size, (GLsizei*)&ret, (GLenum*)binaryFormats, buff);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error geting shader program binary %s\n", gluErrorString(error));
	}

	RELEASE_ARRAY(binaryFormats);

	return ret;
}

int ModuleRenderer3D::GetProgramSize(uint program_id) const
{
	int ret = 0;

	glGetProgramiv(program_id, GL_PROGRAM_BINARY_LENGTH, &ret);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error geting shader program size %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::LoadProgramFromBinary(uint program_id, uint buff_size, const char * buff)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	GLint *binaryFormats = new GLint[formats];
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

	glProgramBinary(program_id, (GLenum)binaryFormats, buff, buff_size);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error loading shader program binary %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::EnableVertexAttributeArray(uint id)
{
	glEnableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error enabling vertex attribute Pointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::DisableVertexAttributeArray(uint id)
{
	glDisableVertexAttribArray(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error disabling vertex attributePointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetVertexAttributePointer(uint id, uint element_size, uint elements_gap, uint infogap)
{
	glVertexAttribPointer(id, element_size, GL_FLOAT, GL_FALSE, elements_gap * sizeof(GLfloat), (void*)(infogap * sizeof(GLfloat)));

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error Setting vertex attributePointer %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformMatrix(uint program, const char * name, float * data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error Setting uniform matrix %s: %s\n", name, gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformForViewAndProjection(uint program, const char * view_name, const char * proj_name)
{
	GLint modelLoc_view = glGetUniformLocation(program, view_name);
	if (modelLoc_view != -1)
		glUniformMatrix4fv(modelLoc_view, 1, GL_FALSE, App->camera->GetViewMatrix());

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error Setting uniform matrix %s %s\n", view_name, gluErrorString(error));
	}

	GLint modelLoc_proj = glGetUniformLocation(program, proj_name);
	if (modelLoc_proj != -1)
		glUniformMatrix4fv(modelLoc_proj, 1, GL_FALSE, App->camera->GetProjectionMatrix());

	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error Setting uniform matrix %s %s\n", proj_name, gluErrorString(error));
	}
}

void ModuleRenderer3D::SetUniformFloat(uint program, const char * name, float data)
{
	GLint modelLoc = glGetUniformLocation(program, name);

	if (modelLoc != -1)
		glUniform1f(modelLoc, data);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error Setting uniform float %s: %s\n", name, gluErrorString(error));
	}
}

uint ModuleRenderer3D::CreateShaderProgram()
{
	uint ret = glCreateProgram();

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error creating shader program %s\n", gluErrorString(error));
	}

	return ret;
}

void ModuleRenderer3D::UseShaderProgram(uint id)
{
	glUseProgram(id);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		CONSOLE_LOG("Error at use shader program: %s\n", gluErrorString(error));
	}
}

void ModuleRenderer3D::AttachShaderToProgram(uint program_id, uint shader_id)
{
	if (program_id > 0 && shader_id > 0)
	{
		glAttachShader(program_id, shader_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error attaching shader %s\n", gluErrorString(error));
		}
	}
}

bool ModuleRenderer3D::LinkProgram(uint program_id)
{
	bool ret = false;

	if (program_id != 0)
	{
		glLinkProgram(program_id);

		GLint success;
		GLint valid;
		glGetProgramiv(program_id, GL_LINK_STATUS, &success);
		glGetProgramiv(program_id, GL_VALIDATE_STATUS, &valid);

		ret = true;

		if (!success || !valid) 
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(program_id, 512, NULL, infoLog);
			CONSOLE_LOG("Shader link error: %s", infoLog);

			ret = false;
		}
	}

	return ret;
}

void ModuleRenderer3D::DeleteProgram(uint program_id)
{
	if (program_id > 0)
	{
		glDeleteProgram(program_id);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			CONSOLE_LOG("Error deleting shader program %s\n", gluErrorString(error));
		}
	}
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

//const float4 ModuleRenderer3D::GetAmbientLight() const
//{
//	//float* color = new float[4];
//	//glLightfv(GL_LIGHT0, GL_AMBIENT, color);
//	//float4 ret = float4(color[0], color[1], color[3], color[1]);
//	//RELEASE_ARRAY(color);
//	return ret;
//}

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

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		CONSOLE_LOG("%d", err);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(size, (GLuint*)&(id));
	glBindTexture(GL_TEXTURE_2D, id);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		CONSOLE_LOG("%d", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture);

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		CONSOLE_LOG("%d", err);
	}


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