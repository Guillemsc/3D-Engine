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

	void SetPoligonModeWireframe() const;
	void SetPoligonModePoints(float point_size = 4.0f) const;
	void SetPoligonModeFill() const;
	void SetDepthTest(const bool& set) const;
	void SetCullFace(const bool& set) const;
	void SetLightingState(const bool& set) const;
	void SetTexture2D(const bool& set) const;
	void SetColorMaterial(const bool& set) const;

	const bool GetPoligonModeWireframe() const;
	const bool GetPoligonModePoints() const;
	const bool GetPoligonModeFill() const;
	const bool GetDepthTest() const;
	const bool GetCullFace() const;
	const bool GetLightingState() const;
	const bool GetTexture2D() const;
	const bool GetColorMaterial() const;

	uint LoadBuffer(float* vertices, uint size);
	uint LoadBuffer(uint* vertices, uint size);
	uint LoadTextureBuffer(const void* texture, uint size, int format, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min);
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