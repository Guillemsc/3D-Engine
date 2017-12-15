#include "ComponentButton.h"
#include "UICanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "SuStoUI.h"
#include "UIButton.h"
#include "imgui.h"
#include "DebugDraw.h"

ComponentButton::ComponentButton(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_BUTTON, owner, unique_id)
{
	LOG_OUTPUT("Component UIImage Created");
	SetName("UIButton");

	owner->SetName("Button");

	GetOwner()->SetIsUI(true);
	TryBindCanvas();
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Start()
{
}

void ComponentButton::Update()
{
	if (button != nullptr)
	{
		button->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(button->GetTransform());

		DebugDraw(button->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentButton::CleanUp()
{
}

void ComponentButton::InspectorDraw(std::vector<Component*> components)
{
	if (button != nullptr)
	{
		ImGui::Text("Canvas Found");

		float2 anchor = float2(button->GetAnchor().x, button->GetAnchor().y);
		if (ImGui::DragFloat2("Anchor", (float*)&anchor, 0.01))
		{
			button->SetAnchor(SuStoVec2(anchor.x, anchor.y));
		}

		float2 pos = float2(button->GetLocalPos().x, button->GetLocalPos().y);
		if (ImGui::DragFloat2("Pos", (float*)&pos, 0.05))
		{
			button->SetLocalPos(SuStoVec2(pos.x, pos.y));
		}

		float2 scale = float2(button->GetLocalScale().x, button->GetLocalScale().y);
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.05))
		{
			button->SetLocalScale(SuStoVec2(scale.x, scale.y));
		}
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = button->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);
}

UIButton * ComponentButton::GetButton()
{
	return nullptr;
}

void ComponentButton::SetIdleImage(uint texture_id, float2 texture_size)
{
	if (button != nullptr)
	{
		button->SetIdleImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

void ComponentButton::SetOverImage(uint texture_id, float2 texture_size)
{
	if (button != nullptr)
	{
		button->SetOverImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

void ComponentButton::SetPressedImage(uint texture_id, float2 texture_size)
{
	if (button != nullptr)
	{
		button->SetPressedImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

void ComponentButton::OnEnable()
{
}

void ComponentButton::OnDisable()
{
}

void ComponentButton::OnChangeParent()
{
	TryBindCanvas();
}

void ComponentButton::OnLoadSerialize(JSON_Doc config)
{
}

void ComponentButton::OnSaveSerialize(JSON_Doc config)
{
}

void ComponentButton::TryBindCanvas()
{
	if (button == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			button = (UIButton*)App->gameobj->GetUIMain()->CreateElement(ElementType::BUTTON, canvas);

			SetIdleImage(4, float2(98, 28));
			SetOverImage(5, float2(98, 28));
			SetPressedImage(6, float2(98, 28));
		}
	}
}
