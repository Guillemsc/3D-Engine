#include "ComponentMaterial.h"
#include "GameObject.h"
#include "App.h"
#include "ResourceTexture.h"
#include "ResourceManager.h"
#include "imgui.h"
#include "JSONLoader.h"

ComponentMaterial::ComponentMaterial(GameObject * owner, std::string unique_id) : Component(ComponentType::MATERIAL, owner, unique_id)
{
	CONSOLE_LOG("Component Material Created");
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
	RemoveTexture();
}

void ComponentMaterial::ChangeTexture(ResourceTexture * text)
{
	if (texture != text)
	{
		RemoveTexture();
		SetTexture(text);
	}
}

void ComponentMaterial::SetTexture(ResourceTexture* text)
{
	RemoveTexture();

	texture = text;

	if (texture != nullptr)
	{
		texture->StartUsing();
		has_texture = true;
	}
}

void ComponentMaterial::RemoveTexture()
{
	if (texture != nullptr)
	{
		texture->StopUsing();

		texture = nullptr;
		has_texture = false;
	}
}

ResourceTexture * ComponentMaterial::GetTexture()
{
	return texture;
}

const bool ComponentMaterial::HasTexture() const
{
	return has_texture;
}

void ComponentMaterial::InspectorDraw(std::vector<Component*> components)
{
	Resource* curr_res = texture;
	if (App->resource_manager->DrawResourceSelector(GetUniqueId().c_str(), "Texture", ResourceType::RT_TEXTURE, curr_res))
	{
		ChangeTexture((ResourceTexture*)curr_res);
	}
	
	if (texture != nullptr)
	{
		ImGui::Text("Unique id: %s", texture->GetUniqueId().c_str());
		ImGui::Text("Used by %d GameObjects", texture->UsedCount());
		ImGui::Text("Id texture: %d", texture->GetTextureId());
		ImGui::Text("Preview:");
		ImGui::Image((ImTextureID)texture->GetTextureId(), ImVec2(200, 200));
	}
}

void ComponentMaterial::OnEnable()
{
}

void ComponentMaterial::OnDisable()
{
}

void ComponentMaterial::OnLoadAbstraction(DataAbstraction& abs)
{
	string texture_id = abs.GetString("texture_id", "no_id");

	ResourceTexture* rtexture = (ResourceTexture*)App->resource_manager->Get(texture_id);

	SetTexture(rtexture);
}

void ComponentMaterial::OnSaveAbstraction(DataAbstraction& abs)
{
	if (has_texture)
	{
		abs.AddString("texture_id", texture->GetUniqueId().c_str());
	}
}
