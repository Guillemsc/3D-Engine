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

void ComponentTransfrom::OnCreate()
{
	ResetLocalMatrix();
}

void ComponentTransfrom::OnDestroy()
{
}

void ComponentTransfrom::OnEnable()
{
}

void ComponentTransfrom::OnDisable()
{
}

float3 ComponentTransfrom::GetLocalPosition()
{
	return float3(local_transform[3][0], local_transform[3][1], local_transform[3][2]);
}

float3 ComponentTransfrom::GetLocalRotationEuler()
{

	return float3();
}

float3 ComponentTransfrom::GetLocalScale()
{
	float x = Sqrt(Pow(local_transform[0][0], 2) + Pow(local_transform[0][1], 2) + Pow(local_transform[0][2], 2));
	float y = Sqrt(Pow(local_transform[1][0], 2) + Pow(local_transform[1][1], 2) + Pow(local_transform[1][2], 2));
	float z = Sqrt(Pow(local_transform[2][0], 2) + Pow(local_transform[2][1], 2) + Pow(local_transform[2][2], 2));

	return float3(x, y, z);
}

void ComponentTransfrom::ResetLocalMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int y = 0; i < 4; i++)
		{
			local_transform[i][y] = 0;
		}
	}
}

void ComponentTransfrom::InspectorDraw()
{
	ImGui::Text(GetName());
}

