#ifndef __ComponentTexture_H__
#define __ComponentTexture_H__

#include "Component.h"

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

public:
	Texture* texture = nullptr;
	bool	 has_texture = false;

};

#endif