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
		App->texture->UnloadTexture(texture);
	}
}

void ComponentTexture::SetTexture(Texture* text)
{
	texture = text;
	has_texture = true;
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
}

void ComponentTexture::OnEnable()
{
}

void ComponentTexture::OnDisable()
{
}