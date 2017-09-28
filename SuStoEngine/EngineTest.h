#ifndef __EngineTest_H__
#define __EngineTest_H__

#include "EditorUI.h"
#include "Module.h"

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
};

#endif