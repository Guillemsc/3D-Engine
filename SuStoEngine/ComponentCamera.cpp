#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"

ComponentCamera::ComponentCamera(GameObject * owner) : Component(CAMERA, owner)
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
	App->renderer3D->GetDebugDraw()->DrawFrustum(corners);
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
