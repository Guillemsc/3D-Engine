#include "ComponentTransform.h"
#include "imgui.h"

ComponentTransfrom::ComponentTransfrom(GameObject * owner) : Component(ComponentType::TRANSFORM, owner)
{
	LOG_OUTPUT("Component Transform Created");
	SetName("Transform");
}

ComponentTransfrom::~ComponentTransfrom()
{
}

void ComponentTransfrom::Start()
{
	ResetTransformMatrix();
}

void ComponentTransfrom::Update()
{
}

void ComponentTransfrom::CleanUp()
{
}

void ComponentTransfrom::OnEnable()
{
}

void ComponentTransfrom::OnDisable()
{
}

const float3 ComponentTransfrom::GetPosition() const
{
	return float3(transform[3][0], transform[3][1], transform[3][2]);
}

const float3 ComponentTransfrom::GetRotationEuler() const
{
	return float3(0, 0, 0);
}

const float3 ComponentTransfrom::GetScale() const
{
	float x = Sqrt(Pow(transform[0][0], 2) + Pow(transform[0][1], 2) + Pow(transform[0][2], 2));
	float y = Sqrt(Pow(transform[1][0], 2) + Pow(transform[1][1], 2) + Pow(transform[1][2], 2));
	float z = Sqrt(Pow(transform[2][0], 2) + Pow(transform[2][1], 2) + Pow(transform[2][2], 2));

	return float3(x, y, z);
}

void ComponentTransfrom::ResetTransformMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			transform[i][y] = 0;
		}
	}
}

void ComponentTransfrom::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());

	float position[3];
	position[0] = GetPosition().x;
	position[1] = GetPosition().y;
	position[2] = GetPosition().z;

	float rotation[3];
	rotation[0] = GetRotationEuler().x;
	rotation[1] = GetRotationEuler().y;
	rotation[2] = GetRotationEuler().z;

	float scale[3];
	scale[0] = GetScale().x;
	scale[1] = GetScale().y;
	scale[2] = GetScale().z;

	ImGui::InputFloat3("Position", position);
	ImGui::InputFloat3("Rotation", rotation);
	ImGui::InputFloat3("Scale", scale);
}

