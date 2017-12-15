#ifndef __COMPONENT_IMAGE_H__
#define __COMPONENT_IMAGE_H__

#include "Component.h"
#include "GeometryMath.h"

class GameObject;
class UIImage;
class UICanvas;

class ComponentImage : public Component
{
public:
	ComponentImage(GameObject* owner, std::string unique_id);
	virtual ~ComponentImage();

	void Start();
	void Update();
	void CleanUp();

	void InspectorDraw(std::vector<Component*> components);

	void SetImage(uint texture_id, float2 texture_size);
	UIImage* GetImage();

private:
	void OnEnable();
	void OnDisable();

	void OnChangeParent();

	virtual void OnLoadSerialize(JSON_Doc config);
	virtual void OnSaveSerialize(JSON_Doc config);

	void TryBindCanvas();

private:
	UIImage* image = nullptr;
};

#endif