#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class ResourceTexture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* owner, std::string unique_id);
	virtual ~ComponentMaterial();

	void Start();
	void Update();
	void CleanUp();

	void SetTexture(ResourceTexture* text);
	void RemoveTexture();
	ResourceTexture* GetTexture();
	const bool HasTexture() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

	void OnLoadSerialize(JSON_Doc config);
	void OnSaveSerialize(JSON_Doc config);

private:
	ResourceTexture* texture = nullptr;
	bool	 has_texture = false;

	float2   text_inspector_size = float2(0, 0);

};

#endif