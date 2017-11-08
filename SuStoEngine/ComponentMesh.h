#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "GeometryLoader.h"

class GameObject;
class ResourceMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner, double id);
	virtual ~ComponentMesh();

	void Start();
	void Update();
	void CleanUp();

	void SetMesh(ResourceMesh* mesh);
	void RemoveMesh();
	ResourceMesh* GetMesh() const;
	const bool HasMesh() const;

	void OnGetBoundingBox(AABB& box);
	void InspectorDraw(std::vector<Component*> components);

	void OnLoadScene(JSON_Doc* config);
	void OnSaveScene(JSON_Doc* config);

private:
	void OnEnable();
	void OnDisable();

public:
private:
	ResourceMesh* mesh = nullptr;
	bool has_mesh = false;;

};

#endif