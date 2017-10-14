#ifndef __ComponentTexture_H__
#define __ComponentTexture_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class Texture;

class ComponentTexture : public Component
{
public:
	ComponentTexture(GameObject* owner);
	virtual ~ComponentTexture();

	void Start();
	void Update();
	void CleanUp();

	void SetTexture(Texture* text);
	Texture* GetTexture();

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