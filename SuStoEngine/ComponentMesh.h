#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "GeometryLoader.h"

class GameObject;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner);
	virtual ~ComponentMesh();

	void Start();
	void Update();
	void CleanUp();

	void SetMesh(Mesh* mesh);
	Mesh* GetMesh() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

public:
private:
	Mesh* mesh = nullptr;
	bool has_mesh = false;;

};

#endif