#include "ComponentImage.h"
#include "App.h"
#include "Globals.h"
#include "ModuleGameObject.h"
#include "SuStoUI.h"
#include "UICanvas.h"
#include "UIImage.h"
#include "imgui.h"
#include "DebugDraw.h"
#include "ModuleRenderer3D.h"

ComponentImage::ComponentImage(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_IMAGE, owner, unique_id)
{
	LOG_OUTPUT("Component UIImage Created");
	SetName("UIImage");

	owner->SetName("Image");

	GetOwner()->SetIsUI(true);
	TryBindCanvas();
}

ComponentImage::~ComponentImage()
{
}

void ComponentImage::Start()
{
	
}

void ComponentImage::Update()
{
	if (image != nullptr)
	{
		image->SetTransform(GetOwner()->transform->GetGlobalTransform());

		GetOwner()->transform->SetGlobalTransform(image->GetTransform());

		DebugDraw(image->GetImage()->GetBbox(), Red, true, 4.0f);
	}
}

void ComponentImage::CleanUp()
{
	App->gameobj->GetUIMain()->DeleteElement(image);
	image = nullptr;
}

void ComponentImage::InspectorDraw(std::vector<Component*> components)
{
	if (image != nullptr)
	{
		ImGui::Text("Canvas Found");

		float2 anchor = float2(image->GetAnchor().x, image->GetAnchor().y);
		if (ImGui::DragFloat2("Anchor", (float*)&anchor, 0.01))
		{
			image->SetAnchor(SuStoVec2(anchor.x, anchor.y));
		}

		float2 pos = float2(image->GetLocalPos().x, image->GetLocalPos().y);
		if (ImGui::DragFloat2("Pos", (float*)&pos, 0.05))
		{
			image->SetLocalPos(SuStoVec2(pos.x, pos.y));
		}

		float2 scale = float2(image->GetLocalScale().x, image->GetLocalScale().y);
		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.05))
		{
			image->SetLocalScale(SuStoVec2(scale.x, scale.y));
		}
	}
	else
		ImGui::Text("No Canvas Found");

	float4x4 anchor_trans = image->GetAnchorPos();
	DebugDraw(float3(anchor_trans[0][3], anchor_trans[1][3], anchor_trans[2][3]), 100);
	
}

void ComponentImage::SetImage(uint texture_id, float2 texture_size)
{
	if (image != nullptr)
	{
		image->SetImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));
	}
}

UIImage * ComponentImage::GetImage()
{
	return image;
}

void ComponentImage::OnEnable()
{
}

void ComponentImage::OnDisable()
{
}

void ComponentImage::OnChangeParent()
{
	TryBindCanvas();
}

void ComponentImage::OnLoadSerialize(JSON_Doc config)
{

}

void ComponentImage::OnSaveSerialize(JSON_Doc config)
{
}

void ComponentImage::TryBindCanvas()
{
	if (image == nullptr)
	{
		UICanvas* canvas = GetOwner()->RecursiveFindCanvasOnParent();

		if (canvas != nullptr)
		{
			image = (UIImage*)App->gameobj->GetUIMain()->CreateElement(ElementType::IMAGE, canvas);

			SetImage(3, float2(747, 600));
		}
	}
}
