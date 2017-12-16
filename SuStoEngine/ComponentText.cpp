#include "ComponentText.h"
#include "UICanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "SuStoUI.h"
#include "UIButton.h"
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
	if (text != nullptr)
	{
	/*	text->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(text->GetTransform());

		DebugDraw(text->GetImage()->GetBbox(), Red, true, 4.0f);*/
	}
}

void ComponentText::CleanUp()
{
}

void ComponentText::InspectorDraw(std::vector<Component*> components)
{
	/*if (text != nullptr)
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
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = text->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);*/
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
}

void ComponentText::OnSaveSerialize(JSON_Doc config)
{
}

void ComponentText::TryBindCanvas()
{
	if (text == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			text = (UIText*)App->gameobj->GetUIMain()->CreateElement(ElementType::TEXT, canvas);

			/*SetIdleImage(4, float2(98, 28));
			SetOverImage(5, float2(98, 28));
			SetPressedImage(6, float2(98, 28));*/
		}
	}
}