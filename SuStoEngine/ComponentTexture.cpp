#include "ComponentTexture.h"
#include "GameObject.h"
#include "App.h"
#include "TextureLoader.h"
#include "imgui.h"

ComponentTexture::ComponentTexture(GameObject * owner) : Component(ComponentType::TEXTURE, owner)
{
	LOG_OUTPUT("Component Texture Created");
	SetName("Texture");
}

ComponentTexture::~ComponentTexture()
{
}

void ComponentTexture::Start()
{
}

void ComponentTexture::Update()
{
}

void ComponentTexture::CleanUp()
{
	if (has_texture)
	{
		texture->DeleteUser();

		if(!texture->IsUsed())
			App->texture->UnloadTexture(texture);
	}
}

void ComponentTexture::SetTexture(Texture* text)
{
	if (text != nullptr && texture != text)
	{
		texture = text;
		text->AddUser();
		has_texture = true;
	}
}

Texture * ComponentTexture::GetTexture()
{
	return texture;
}

void ComponentTexture::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());

	if (!has_texture)
	{
		ImGui::Text("No texture loaded");
		return;
	}

	ImGui::Text("Id texture: %d", texture->GetId());
	ImGui::Text("Used by %d GameObjects", texture->UsedBy());
}

void ComponentTexture::OnEnable()
{
}

void ComponentTexture::OnDisable()
{
}