#ifndef __MODULE_RENDERER_H__
#define __MODULE_RENDERER_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "DebugDraw.h"

#include "Glew\Include\glew.h"
#include "SDL/include/SDL_opengl.h"
#include "GeometryMath.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define MAX_LIGHTS 8

class FBO;
class SuStoUIMain;
class DebugDraw;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	void OnResize(int width, int height);
	uint GetScreenTexture();

	void SetPoligonModeWireframe() const;
	void SetPoligonModePoints(float point_size = 4.0f) const;
	void SetPoligonModeFill() const;
	void SetDepthTest(const bool& set) const;
	void SetCullFace(const bool& set) const;
	void SetLightingState(const bool& set) const;
	void SetTexture2D(const bool& set) const;
	void SetColorMaterial(const bool& set) const;
	void SetAmbientLight(const bool & enabled, const float color[4]) const;
	void SetDiffuseLight(const bool & enabled, const float color[4]) const;
	void SetSpecularLight(const bool & enabled, const float color[4]) const;

	const bool GetPoligonModeWireframe() const;
	const bool GetPoligonModePoints() const;
	const bool GetPoligonModeFill() const;
	const bool GetDepthTest() const;
	const bool GetCullFace() const;
	const bool GetLightingState() const;
	const bool GetTexture2D() const;
	const bool GetColorMaterial() const;
	//const float4 GetAmbientLight() const;

	uint LoadBuffer(float* elements, uint size);
	uint LoadBuffer(uint* elements, uint size);
	uint LoadTextureBuffer(const void* texture, uint size, int format, int width, int height, uint wrap_s, uint wrap_t, uint mag, uint min);
	void UnloadBuffer(uint id, uint size);
	void UnloadTextureBuffer(uint id, uint size);

	void DrawGrid(int HALF_GRID_SIZE);

private:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	FBO* fbo_texture = nullptr;
	uint texture_id = 0;

	bool wireframe = false;
	bool points = false;
	bool fill = false;
};

#endif // !__MODULE_RENDERER_H__