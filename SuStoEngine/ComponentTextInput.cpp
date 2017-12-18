#include "ComponentTextInput.h"
#include "UITextInput.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "UICanvas.h"
#include "imgui.h"
#include "DebugDraw.h"
#include "Color.h"
#include "UITextInput.h"
#include "UIElement.h"


ComponentTextInput::ComponentTextInput(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_TEXT_INPUT, owner, unique_id)
{
	LOG_OUTPUT("Component UITextInput Created");
	SetName("UITextInput");

	owner->SetName("TextInput");

	GetOwner()->SetIsUI(true);
	TryBindCanvas();
}

ComponentTextInput::~ComponentTextInput()
{
}

void ComponentTextInput::Start()
{
}

void ComponentTextInput::Update()
{
	if (text != nullptr)
	{
		text->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(text->GetTransform());

		DebugDraw(text->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentTextInput::CleanUp()
{
	App->gameobj->GetUIMain()->DeleteElement(text);
	text = nullptr;
}

void ComponentTextInput::InspectorDraw(std::vector<Component*> components)
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

		ImGui::Separator();

		if (text->OnFocus())
		{
			ImGui::Text("Focused");
		}
	}
	ImGui::Separator();
}

UITextInput * ComponentTextInput::GetText()
{
	return text;
}

void ComponentTextInput::OnEnable()
{
}

void ComponentTextInput::OnDisable()
{
}

void ComponentTextInput::OnChangeParent()
{
	TryBindCanvas();
}

void ComponentTextInput::OnLoadSerialize(JSON_Doc config)
{
}

void ComponentTextInput::OnSaveSerialize(JSON_Doc config)
{
}

void ComponentTextInput::TryBindCanvas()
{
	if (text == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			text = (UITextInput*)App->gameobj->GetUIMain()->CreateElement(ElementType::TEXT_INPUT, canvas);
		}
	}
}
