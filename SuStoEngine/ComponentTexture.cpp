#include "ComponentTexture.h"
#include "GameObject.h"
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
}

void ComponentTexture::InspectorDraw(std::vector<Component*> components)
{
	ImGui::Text(GetName());
}

void ComponentTexture::OnEnable()
{
}

void ComponentTexture::OnDisable()
{
}