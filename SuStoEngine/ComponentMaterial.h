#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Texture;

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* owner);
	virtual ~ComponentMaterial();

	void Start();
	void Update();
	void CleanUp();

	void SetTexture(Texture* text);
	Texture* GetTexture();
	const bool HasTexture() const;

	void InspectorDraw(std::vector<Component*> components);

private:
	void OnEnable();
	void OnDisable();

private:
	Texture* texture = nullptr;
	bool	 has_texture = false;

	float2   text_inspector_size = float2(0, 0);

};

#endif