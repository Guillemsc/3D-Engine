#include "ComponentText.h"
#include "UICanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "SuStoUI.h"
#include "UIText.h"
#include "imgui.h"
#include "DebugDraw.h"

ComponentText::ComponentText(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_TEXT, owner, unique_id)
{
	LOG_OUTPUT("Component UIText Created");
	SetName("UIText");

	owner->SetName("Text");

	GetOwner()->SetIsUI(true);
	TryBindCanvas();
}

ComponentText::~ComponentText()
{
}

void ComponentText::Start()
{
}

void ComponentText::Update()
{
	if (to_load)
	{
		OnLoadSerialize(config);
		to_load = false;
	}

	if (text != nullptr)
	{
		text->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(text->GetTransform());

		DebugDraw(text->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentText::CleanUp()
{
}

void ComponentText::InspectorDraw(std::vector<Component*> components)
{
	if (text != nullptr)
	{
		ImGui::Text("Canvas Found");

		float2 anchor = float2(text->GetAnchor().x, text->GetAnchor().y);
		if (ImGui::DragFloat2("Anchor", (float*)&anchor, 0.01))
		{
			text->SetAnchor(SuStoVec2(anchor.x, anchor.y));
		}

		float2 pos = float2(text->GetLocalPos().x, text->GetLocalPos().y);
		if (ImGui::DragFloat2("Pos", (float*)&pos, 0.05))
		{
			text->SetLocalPos(SuStoVec2(pos.x, pos.y));
		}

		float2 scale = float2(text->GetLocalScale().x, text->GetLocalScale().y);
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.05))
		{
			text->SetLocalScale(SuStoVec2(scale.x, scale.y));
		}
		char buffer[255];
		if (ImGui::InputText("Text", buffer, 255))
		{
			text->SetText(buffer);
		}
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = text->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);
}

void ComponentText::SetText(const char * t)
{
	if (text != nullptr)
	{
		text->SetText(t);
	}
}

UIText * ComponentText::GetText()
{
	return text;
}

void ComponentText::OnEnable()
{
}

void ComponentText::OnDisable()
{
}

void ComponentText::OnChangeParent()
{
	TryBindCanvas();
}

void ComponentText::OnLoadSerialize(JSON_Doc config)
{
	if (text != nullptr)
	{
		SuStoVec2 anchor(config.GetNumber("anchor_x", 0), config.GetNumber("anchor_y", 0));
		SuStoVec2 pos(config.GetNumber("pos_x", 0), config.GetNumber("pos_y", 0));
		SuStoVec2 scale(config.GetNumber("scale_x", 0), config.GetNumber("scale_y", 0));

		text->SetAnchor(anchor);
		text->SetLocalPos(pos);
		text->SetLocalScale(scale);
	}

	if (!to_load)
	{
		this->config = config;
		to_load = true;
	}
}

void ComponentText::OnSaveSerialize(JSON_Doc config)
{
	config.SetNumber("anchor_x", text->GetAnchor().x);
	config.SetNumber("anchor_y", text->GetAnchor().y);
	config.SetNumber("pos_x", text->GetLocalPos().x);
	config.SetNumber("pos_y", text->GetLocalPos().y);
	config.SetNumber("scale_x", text->GetLocalScale().x);
	config.SetNumber("scale_y", text->GetLocalScale().y);
}

void ComponentText::TryBindCanvas()
{
	if (text == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			text = (UIText*)App->gameobj->GetUIMain()->CreateElement(ElementType::TEXT, canvas);
		}
	}
}