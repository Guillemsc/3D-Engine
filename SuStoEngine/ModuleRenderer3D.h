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

	void RenderScene();

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

	// OpenGl control -----------------------------------------
	void SetViewport(uint start_x, uint start_y, uint width, uint height);
	void GetViewport(uint& start_x, uint& start_y, uint& width, uint& height);
	void Clear(uint buffer);

	uint GenBuffer() const;

	void BindArrayBuffer(uint id) const;
	void BindElementArrayBuffer(uint id) const;

	void RenderElement(uint num_indices) const;

	void UnbindArraybuffer() const;
	void UnbindElementArrayBuffer() const;

	void EnableState(GLenum type) const;
	void DisableState(GLenum type) const;

	void SetVertexPointer() const;
	void SetNormalsPointer() const;
	void SetTexCoordPointer();

	uint GenTexture() const;
	void BindTexture(uint id) const;
	void BindTexture(uint target, uint id) const;
	void UnbindTexture() const;
	void UnbindTexture(uint target) const;
	void DeleteTexture(uint& id) const;

	uint GenRenderBuffer() const;
	void BindRenderBuffer(uint id) const;
	void UnbindRenderBuffer() const;

	void Set2DMultisample(uint samples, uint width, uint height);
	void SetFrameBufferTexture2D(uint target, uint id);
	void RenderStorageMultisample(uint samples, uint width, uint height);

	void LoadArrayToVRAM(uint size, float* values, GLenum type = GL_STATIC_DRAW) const;
	void LoadArrayToVRAM(uint size, uint* values, GLenum type = GL_STATIC_DRAW) const;
	uint LoadTextureToVRAM(uint w, uint h, GLubyte* tex_data, GLint format) const;

	void PushMatrix();
	void PopMatrix();
	void MultMatrix(float* matrix);
	// --------------------------------------------------------

	// Shaders ------------------------------------------------
	uint GenVertexArrayBuffer() const;
	void BindVertexArrayBuffer(uint id) const;
	void UnbindVertexArrayBuffer() const;

	uint GenFrameBuffer() const;
	void BindFrameBuffer(uint id) const;
	void RenderFrameBuffer(uint id) const;
	void UnbindFrameBuffer() const;
	uint CheckFrameBufferStatus();
	void DeleteFrameBuffer(uint& id);

	uint CreateVertexShader(const char* source);
	uint CreateFragmentShader(const char* source);
	void DeleteShader(uint shader_id);

	uint GetProgramBinary(uint program_id, uint buff_size, char* buff) const;
	int GetProgramSize(uint program_id) const;
	void LoadProgramFromBinary(uint program_id, uint buff_size, const char* buff);

	void EnableVertexAttributeArray(uint id);
	void DisableVertexAttributeArray(uint id);
	void SetVertexAttributePointer(uint id, uint element_size, uint elements_gap, uint infogap);

	void SetUniformMatrix(uint program, const char* name, float* data);
	void SetUniformForViewAndProjection(uint program, const char* view_name, const char* proj_name);

	void SetUniformFloat(uint program, const char* name, float data);
	void SetUniformBool(uint program, const char* name, bool data);

	uint CreateShaderProgram();
	void UseShaderProgram(uint id);
	void AttachShaderToProgram(uint program_id, uint shader_id);
	bool LinkProgram(uint program_id);
	void DeleteProgram(uint program_id);
	// --------------------------------------------------------

	// Deprecated ---------------------------------------------
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
	// -------------------------------------------------

private:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool wireframe = false;
	bool points = false;
	bool fill = false;
};

#endif // !__MODULE_RENDERER_H__