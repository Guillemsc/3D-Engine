#ifndef __ComponentPrimitive_H__
#define __ComponentPrimitive_H__

#include "Component.h"

class GameObject;

enum PrimitiveType
{
	NO_VALUE,
	VCUBE,
	ICUBE,
	SPHERE,
};

class ComponentPrimitive : public Component
{
public:
	ComponentPrimitive(GameObject* owner);
	virtual ~ComponentPrimitive();

	void Start();
	void Update();
	void CleanUp();

	void SetPrimitive(PrimitiveType type);
	
	void InspectorDraw(std::vector<Component*> components);

private:
	void VertexCube();
	void IndexCube();
	void Sphere();

	void OnEnable();
	void OnDisable();

public:
private:
	PrimitiveType type = VCUBE;

	// Cube vertices
	float*	      vCube_vertices = nullptr;
	int			  vCube_vertices_count = 0;

	// Cube index
	float*	      iCube_vertices = nullptr;
	int			  iCube_vertices_count = 0;
	unsigned int* iCube_index = nullptr;
	int			  iCube_index_count = 0;

	// Sphere
	std::vector<float> vertices;
	std::vector<uint>  indices;
};

#endif