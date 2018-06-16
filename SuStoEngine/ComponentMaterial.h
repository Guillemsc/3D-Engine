#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"
#include "GeometryMath.h"


class GameObject;
class ResourceTexture;
class ResourceShader;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* owner, std::string unique_id);
	virtual ~ComponentMaterial();

	void Start();
	void Update();
	void CleanUp();

	void ChangeTexture(ResourceTexture* text);
	void SetTexture(ResourceTexture* text);
	void RemoveTexture();
	ResourceTexture* GetTexture();
	const bool HasTexture() const;

	ResourceShader* GetShader();

	void InspectorDraw(std::vector<Component*> components);

	void OnLoadAbstraction(DataAbstraction& abs);
	void OnSaveAbstraction(DataAbstraction& abs);

private:
	void OnEnable();
	void OnDisable();

private:
	ResourceTexture* texture = nullptr;
	bool	 has_texture = false;
	
	ResourceShader* shader = nullptr;

};

#endif