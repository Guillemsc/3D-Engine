#include "ComponentMaterial.h"
#include "GameObject.h"
#include "App.h"
#include "TextureLoader.h"
#include "imgui.h"

ComponentMaterial::ComponentMaterial(GameObject * owner) : Component(ComponentType::MATERIAL, owner)
{
	LOG_OUTPUT("Component Material Created");
	SetName("Material");
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Start()
{
}

void ComponentMaterial::Update()
{
}

void ComponentMaterial::CleanUp()
{
	if (has_texture)
	{
		texture->DeleteUser();

		if(!texture->IsUsed())
			App->texture->UnloadTexture(texture);
	}
}

void ComponentMaterial::SetTexture(Texture* text)
{
	if (text != nullptr && texture != text)
	{
		texture = text;
		text->AddUser();
		has_texture = true;
	}
}

Texture * ComponentMaterial::GetTexture()
{
	return texture;
}

const bool ComponentMaterial::HasTexture() const
{
	return has_texture;
}

void ComponentMaterial::InspectorDraw(std::vector<Component*> components)
{
	if (!has_texture)
	{
		ImGui::Text("No texture loaded");
		return;
	}

	ImGui::Text("Id texture: %d", texture->GetId());
	ImGui::Text("Used by %d GameObjects", texture->UsedBy());
	ImGui::Text("Preview:");
	ImGui::Image((ImTextureID)texture->GetId(), ImVec2(200, 200));
}

void ComponentMaterial::OnEnable()
{
}

void ComponentMaterial::OnDisable()
{
}