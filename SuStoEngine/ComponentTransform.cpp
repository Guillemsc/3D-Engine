#include "ComponentTransform.h"
#include "imgui.h"

ComponentTransform::ComponentTransform(GameObject * owner) : Component(ComponentType::TRANSFORM, owner)
{
	LOG_OUTPUT("Component Transform Created");
	SetName("Transform");
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Start()
{
	ResetTransformMatrix();
}

void ComponentTransform::Update()
{
}

void ComponentTransform::CleanUp()
{
}

const void ComponentTransform::SetPosition(float3 & pos)
{
	transform[3][0] = pos.x;
	transform[3][1] = pos.y;
	transform[3][2] = pos.z;
}

void ComponentTransform::OnEnable()
{
}

void ComponentTransform::OnDisable()
{
}

const float3 ComponentTransform::GetPosition() const
{
	return float3(transform[3][0], transform[3][1], transform[3][2]);
}

const float3 ComponentTransform::GetRotationEuler() const
{
	return float3(0, 0, 0);
}

const float3 ComponentTransform::GetScale() const
{
	float x = Sqrt(Pow(transform[0][0], 2) + Pow(transform[0][1], 2) + Pow(transform[0][2], 2));
	float y = Sqrt(Pow(transform[1][0], 2) + Pow(transform[1][1], 2) + Pow(transform[1][2], 2));
	float z = Sqrt(Pow(transform[2][0], 2) + Pow(transform[2][1], 2) + Pow(transform[2][2], 2));

	return float3(x, y, z);
}

void ComponentTransform::ResetTransformMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; y < 4; y++)
		{
			transform[i][y] = 0;
		}
	}
}

void ComponentTransform::InspectorDraw(std::vector<Component*> components)
{
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

