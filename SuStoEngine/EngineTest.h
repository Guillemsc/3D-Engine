#ifndef __EngineTest_H__
#define __EngineTest_H__

#include "EditorUI.h"
#include "Module.h"

enum polygon_mode 
{
	gl_fill,
	gl_line,
	gl_point
};

class EngineTest : public EditorElement
{
public:
	EngineTest(bool start_enabled = true);
	virtual ~EngineTest();

	void Start();
	void Draw();

private:
	void GenerateRandomNumbers(float2 range, int quantity);
	void GeometryMathTest();
	void OpenGLOptions();

private:
	// Random generator
	float2 range_demo; // for the random generation demo
	int quantity_demo = 1; // number of random generations of the demo

	// Geometry math test
	float vec3a[3]						= { 0.0f, 0.0f, 0.0f };
	float dist							= 0;
	bool  contact						= false;
	bool  contact_sphere_sphere			= false;
	bool  contact_sphere_capsules		= false;
	bool  contact_aabb_aabb				= false;
	bool  contact_obb_obb				= false;
	bool  contact_aabb_ray				= false;

	// GL_DEPTH_TEST		GL_CULL_FACE		GL_LIGHTING
	// GL_COLOR_MATERIAL	GL_TEXTURE_2D		two other
	// Wireframe
	bool gl_depth						= true;
	bool gl_cull_face					= false;
	bool gl_lighting					= true;
	bool gl_color_material				= true;
	bool gl_texture_2d					= false;
	bool fill_mode						= true;
	bool wireframe_mode					= false;
	bool point_mode						= false;

	polygon_mode poly_mode = polygon_mode::gl_fill;

};

#endif