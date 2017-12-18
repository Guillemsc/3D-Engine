#include "ComponentButton.h"
#include "UICanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "SuStoUI.h"
#include "UIButton.h"
#include "imgui.h"
#include "DebugDraw.h"
#include "ResourceTextureLoader.h"

ComponentButton::ComponentButton(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_BUTTON, owner, unique_id)
{
	LOG_OUTPUT("Component UIImage Created");
	SetName("UIButton");

	owner->SetName("Button");

	GetOwner()->SetIsUI(true);
	TryBindCanvas(true);
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::Start()
{
}

void ComponentButton::Update()
{
	if (to_load)
	{
		OnLoadSerialize(config);
		to_load = false;
	}

	if (button != nullptr)
	{
		button->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(button->GetTransform());

		DebugDraw(button->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentButton::CleanUp()
{
	App->gameobj->GetUIMain()->DeleteElement(button);
	button = nullptr;
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

		ImGui::Separator();

		if (button->GetState() == ButtonState::IDLE)
		{
			ImGui::Text("idle");
		}
		if (button->GetState() == ButtonState::OVER)
		{
			ImGui::Text("over");
		}
		if (button->GetState() == ButtonState::PRESSED)
		{
			ImGui::Text("pressed");
		}
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = button->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);
}

UIButton * ComponentButton::GetButton()
{
	return button;
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
	TryBindCanvas(false);
}

void ComponentButton::OnLoadSerialize(JSON_Doc config)
{
	if (button != nullptr)
	{
		SuStoVec2 anchor(config.GetNumber("anchor_x", 0), config.GetNumber("anchor_y", 0));
		SuStoVec2 pos(config.GetNumber("pos_x", 0), config.GetNumber("pos_y", 0));
		SuStoVec2 scale(config.GetNumber("scale_x", 0), config.GetNumber("scale_y", 0));

		button->SetAnchor(anchor);
		button->SetLocalPos(pos);
		button->SetLocalScale(scale);

		button->SetIdleImage(config.GetNumber("idle_id"), SuStoVec2(config.GetNumber("idle_size_x"), config.GetNumber("idle_size_y")));
		button->SetOverImage(config.GetNumber("over_id"), SuStoVec2(config.GetNumber("over_size_x"), config.GetNumber("over_size_y")));
		button->SetPressedImage(config.GetNumber("pressed_id"), SuStoVec2(config.GetNumber("pressed_size_x"), config.GetNumber("pressed_size_y")));
	}

	if (!to_load)
	{
		this->config = config;
		to_load = true;
	}
}

void ComponentButton::OnSaveSerialize(JSON_Doc config)
{
	config.SetNumber("anchor_x", button->GetAnchor().x);
	config.SetNumber("anchor_y", button->GetAnchor().y);
	config.SetNumber("pos_x", button->GetLocalPos().x);
	config.SetNumber("pos_y", button->GetLocalPos().y);
	config.SetNumber("scale_x", button->GetLocalScale().x);
	config.SetNumber("scale_y", button->GetLocalScale().y);

	config.SetNumber("idle_id", button->GetIdleId());
	config.SetNumber("idle_size_x", button->GetIdleSize().x);
	config.SetNumber("idle_size_y", button->GetIdleSize().y);
	config.SetNumber("over_id", button->GetOverId());
	config.SetNumber("over_size_x", button->GetOverSize().x);
	config.SetNumber("over_size_y", button->GetOverSize().y);
	config.SetNumber("pressed_id", button->GetPressedId());
	config.SetNumber("pressed_size_x", button->GetPressedSize().x);
	config.SetNumber("pressed_size_y", button->GetPressedSize().y);
}

void ComponentButton::TryBindCanvas(bool first_time)
{
	if (button == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			button = (UIButton*)App->gameobj->GetUIMain()->CreateElement(ElementType::BUTTON, canvas);

			if (first_time)
			{
				SetIdleImage(4, float2(98, 28));
				SetOverImage(5, float2(98, 28));
				SetPressedImage(6, float2(98, 28));
			}
			
			SetIdleImage(config.GetNumber("idle_id"), float2(config.GetNumber("idle_size_x"), config.GetNumber("idle_size_y")));
			SetOverImage(config.GetNumber("over_id"), float2(config.GetNumber("over_size_x"), config.GetNumber("over_size_y")));
			SetPressedImage(config.GetNumber("pressed_id"), float2(config.GetNumber("pressed_size_x"), config.GetNumber("pressed_size_y")));
		}
	}
}
