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
	CONSOLE_LOG("Component Camera Created");
	SetName("Camera");

	camera = App->camera->CreateCamera();
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
	App->camera->DestroyCamera(camera);
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
	bool frustum_culling = camera->GetFrustumCulling();
	if (ImGui::Checkbox("Frustum culling", &frustum_culling))
	{
		camera->SetFrustumCulling(frustum_culling);
	}
}

void ComponentCamera::OnEnable()
{
}

void ComponentCamera::OnDisable()
{
}

void ComponentCamera::OnLoadAbstraction(DataAbstraction& abs)
{
	float far_plane_distance = abs.GetFloat("far_plane_distance", 1000);
	float near_plane_distance = abs.GetFloat("near_plane_distance", 0.1f);
	float fov = abs.GetFloat("fov", 60.0f);

	camera->SetFarPlaneDistance(far_plane_distance);
	camera->SetNearPlaneDistance(near_plane_distance);
	camera->SetFOV(fov);
}

void ComponentCamera::OnSaveAbstraction(DataAbstraction& abs)
{
	abs.AddFloat("far_plane_distance", camera->GetFarPlaneDistance());
	abs.AddFloat("near_plane_distance", camera->GetNearPlaneDistance());
	abs.AddFloat("fov", camera->GetVerticalFOV());
}