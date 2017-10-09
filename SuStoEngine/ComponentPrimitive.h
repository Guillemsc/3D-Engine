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

	void Update();
	
	void Cube();
	void InspectorDraw(std::vector<Component*> components);

private:
	void OnCreate();
	void OnDestroy();
	void OnEnable();
	void OnDisable();

public:
private:
	PrimitiveType type = CUBE;
};

#endif