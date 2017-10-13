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

	void SetPoligonModeWireframe();
	void SetPoligonModePoints(float point_size = 4.0f);
	void SetPoligonModeFill();
	void SetDepthTest(bool set);
	void SetCullFace(bool set);
	void SetLightingState(bool set);
	void SetTexture2D(bool set);
	void SetColorMaterial(bool set);

	bool GetPoligonModeWireframe();
	bool GetPoligonModePoints();
	bool GetPoligonModeFill();
	bool GetDepthTest();
	bool GetCullFace();
	bool GetLightingState();
	bool GetTexture2D();
	bool GetColorMaterial();

	uint LoadBuffer(float* vertices, uint size);
	uint LoadBuffer(uint* vertices, uint size);
	uint LoadTextureBuffer(uint * texture, uint size, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min);
	void UnloadBuffer(uint id, uint size);

	void DrawPlane(float pos_x, float pos_y, float pos_z, int width, int height);

	void DrawGrid(int HALF_GRID_SIZE);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	FBO* fbo_texture;
	uint texture_id;

	bool wireframe = false;
	bool points = false;
	bool fill = false;
};