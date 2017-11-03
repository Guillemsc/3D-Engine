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
	const bool HasMesh() const;

	void OnGetBoundingBox(AABB& box);
	void InspectorDraw(std::vector<Component*> components);

	void LoadScene(JSON_Doc* config);
	void SaveScene(JSON_Doc* config, string root);

private:
	void OnEnable();
	void OnDisable();

public:
private:
	Mesh* mesh = nullptr;
	bool has_mesh = false;;

};

#endif