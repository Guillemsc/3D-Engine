#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class FBO;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Awake();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);

	int LoadBuffer(float* vertices, int size);
	void DrawVertexBuffer(int id, int size);
	void DrawIndexBuffer(unsigned int glmode, int index_id, int index_size, int vertex_id);

	void DrawPlane(float pos_x, float pos_y, float pos_z, int width, int height);

	void DrawGrid(int HALF_GRID_SIZE);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	FBO* fbo_texture;
	uint texture_id;

};