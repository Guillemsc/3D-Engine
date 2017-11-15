#include "ComponentCamera.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"
#include "App.h"
#include "ModuleRenderer3D.h"
#include "DebugDraw.h"
#include "JSONLoader.h"
#include "imgui.h"

ComponentCamera::ComponentCamera(GameObject * owner, std::string unique_id) : Component(CAMERA, owner, unique_id)
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
	RELEASE(camera);
}

Camera3D * ComponentCamera::GetCamera() const
{
	return camera;
}

void ComponentCamera::InspectorDraw(std::vector<Component*> components)
{
	float near_plane = camera->GetNearPlaneDistance();
	if (ImGui::DragFloat("Near plane", &near_plane, 0.2f, 0.01f, 1000))
	{
		camera->SetNearPlaneDistance(near_plane);
	}

	float far_plane = camera->GetFarPlaneDistance();
	if (ImGui::DragFloat("Far plane", &far_plane, 10, near_plane, 10000))
	{
		camera->SetFarPlaneDistance(far_plane);
	}

	float fov = camera->GetVerticalFOV();
	if (ImGui::DragFloat("Field of view", &fov, 1, 1, 179.9f))
	{
		camera->SetFOV(fov);
	}
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
