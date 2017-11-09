#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "JSONLoader.h"

ComponentCamera::ComponentCamera(GameObject * owner, double id) : Component(CAMERA, owner, id)
{
	LOG_OUTPUT("Component Camera Created");
	SetName("Camera");

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
	camera->SetPosition(GetOwner()->transform->GetPosition());
	camera->SetZDir(GetOwner()->transform->GetGlobalTransform().WorldZ());
	camera->SetYDir(GetOwner()->transform->GetGlobalTransform().WorldY());

	float3 corners[8];
	camera->GetCorners(corners);	
	DebugDraw(camera->GetFrustum(), Color(255, 255, 255), true, 5);
}

void ComponentCamera::CleanUp()
{
	delete camera;
}

Camera3D * ComponentCamera::GetCamera() const
{
	return camera;
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

void ComponentCamera::OnLoadScene(JSON_Doc * config)
{
}

void ComponentCamera::OnSaveScene(JSON_Doc * config)
{
	config->SetNumber("far_plane_distance", camera->GetFarPlaneDistance());
	config->SetNumber("near_plane_distance", camera->GetNearPlaneDistance());
	config->SetNumber("fov", camera->GetVerticalFOV());
}
