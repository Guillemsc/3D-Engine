#include "ComponentCheckBox.h"
#include "UICanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "SuStoUI.h"
#include "UICheckBox.h"
#include "imgui.h"
#include "DebugDraw.h"

ComponentCheckBox::ComponentCheckBox(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_CHECKBOX, owner, unique_id)
{
	LOG_OUTPUT("Component UICheckBox Created");
	SetName("UICheckBox");

	owner->SetName("CheckBox");

	GetOwner()->SetIsUI(true);
	TryBindCanvas();
}

ComponentCheckBox::~ComponentCheckBox()
{
}

void ComponentCheckBox::Start()
{
}

void ComponentCheckBox::Update()
{
	if (to_load)
	{
		OnLoadSerialize(config);
		to_load = false;
	}

	if (checkbox != nullptr)
	{
		checkbox->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(checkbox->GetTransform());

		DebugDraw(checkbox->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentCheckBox::CleanUp()
{
	App->gameobj->GetUIMain()->DeleteElement(checkbox);
	checkbox = nullptr;
}

void ComponentCheckBox::InspectorDraw(std::vector<Component*> components)
{
	if (checkbox != nullptr)
	{
		ImGui::Text("Canvas Found");

		float2 anchor = float2(checkbox->GetAnchor().x, checkbox->GetAnchor().y);
		if (ImGui::DragFloat2("Anchor", (float*)&anchor, 0.01))
		{
			checkbox->SetAnchor(SuStoVec2(anchor.x, anchor.y));
		}

		float2 pos = float2(checkbox->GetLocalPos().x, checkbox->GetLocalPos().y);
		if (ImGui::DragFloat2("Pos", (float*)&pos, 0.05))
		{
			checkbox->SetLocalPos(SuStoVec2(pos.x, pos.y));
		}

		float2 scale = float2(checkbox->GetLocalScale().x, checkbox->GetLocalScale().y);
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.05))
		{
			checkbox->SetLocalScale(SuStoVec2(scale.x, scale.y));
		}
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = checkbox->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);
}

UICheckBox * ComponentCheckBox::GetCheckBox()
{
	return checkbox;
}

void ComponentCheckBox::SetCheckFalseImage(uint texture_id, float2 texture_size)
{
	if (checkbox != nullptr)
	{
		checkbox->SetCheckFalseImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

void ComponentCheckBox::SetCheckTrueImage(uint texture_id, float2 texture_size)
{
	if (checkbox != nullptr)
	{
		checkbox->SetCheckTrueImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

void ComponentCheckBox::OnEnable()
{
}

void ComponentCheckBox::OnDisable()
{
}

void ComponentCheckBox::OnChangeParent()
{
	TryBindCanvas();
}

void ComponentCheckBox::OnLoadSerialize(JSON_Doc config)
{
	if (checkbox != nullptr)
	{
		SuStoVec2 anchor(config.GetNumber("anchor_x", 0), config.GetNumber("anchor_y", 0));
		SuStoVec2 pos(config.GetNumber("pos_x", 0), config.GetNumber("pos_y", 0));
		SuStoVec2 scale(config.GetNumber("scale_x", 0), config.GetNumber("scale_y", 0));

		checkbox->SetAnchor(anchor);
		checkbox->SetLocalPos(pos);
		checkbox->SetLocalScale(scale);
	}

	if (!to_load)
	{
		this->config = config;
		to_load = true;
	}
}

void ComponentCheckBox::OnSaveSerialize(JSON_Doc config)
{
	config.SetNumber("anchor_x", checkbox->GetAnchor().x);
	config.SetNumber("anchor_y", checkbox->GetAnchor().y);
	config.SetNumber("pos_x", checkbox->GetLocalPos().x);
	config.SetNumber("pos_y", checkbox->GetLocalPos().y);
	config.SetNumber("scale_x", checkbox->GetLocalScale().x);
	config.SetNumber("scale_y", checkbox->GetLocalScale().y);
}

void ComponentCheckBox::TryBindCanvas()
{
	if (checkbox == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			checkbox = (UICheckBox*)App->gameobj->GetUIMain()->CreateElement(ElementType::CHECKBOX, canvas);
			
			SetCheckFalseImage(7, float2(19, 19));
			SetCheckTrueImage(8, float2(19, 19));
		}
	}
}
