#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"

class GameObject;
class ResourceMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* owner, std::string unique_id);
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

	void OnLoadSerialize(JSON_Doc config);
	void OnSaveSerialize(JSON_Doc config);

private:
	void OnEnable();
	void OnDisable();

public:
private:
	ResourceMesh* mesh = nullptr;
	bool has_mesh = false;;

};

#endif