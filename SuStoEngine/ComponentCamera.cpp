#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera(GameObject * owner) : Component(CAMERA, owner)
{
	camera = new Camera3D();
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Start()
{

}

void ComponentCamera::Update()
{

}

void ComponentCamera::CleanUp()
{
	delete camera;
}

void ComponentCamera::InspectorDraw(std::vector<Component*> components)
{
}

void ComponentCamera::OnEnable()
{
}

void ComponentCamera::OnDisable()
{
}
