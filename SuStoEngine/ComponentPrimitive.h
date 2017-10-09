#ifndef __ComponentPrimitive_H__
#define __ComponentPrimitive_H__

#include "Component.h"

class GameObject;

enum PrimitiveType
{
	CUBE,
};

class ComponentPrimitive : public Component
{
public:
	ComponentPrimitive(GameObject* owner);
	virtual ~ComponentPrimitive();

	void Start();
	void Update();
	void CleanUp();
	
	void VertexCube();
	void IndexCube();
	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

public:
private:
	PrimitiveType type = CUBE;
	float*	      vertices = nullptr;
	int			  vertices_count = 0;

	float*		  index = nullptr;
	int			  index_count = 0;
};

#endif