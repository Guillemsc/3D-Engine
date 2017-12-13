#include "ComponentImage.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "SuStoUI.h"
#include "UICanvas.h"
#include "UIImage.h"
#include "imgui.h"

ComponentImage::ComponentImage(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_IMAGE, owner, unique_id)
{
	LOG_OUTPUT("Component UIImage Created");
	SetName("UIImage");

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

		//float4x4 parent_pos = GetOwner()->GetParentTransform();

		//float4x4 new_pos = image->GetAnchorPos();
		//new_pos[3][0] += image->GetLocalPos().x;
		//new_pos[3][1] += image->GetLocalPos().y;

		//float4x4 new_local = new_pos * parent_pos.Inverted();

		//GetOwner()->transform->SetLocalTransform(new_local);
	}
}

void ComponentImage::CleanUp()
{
}

void ComponentImage::InspectorDraw(std::vector<Component*> components)
{
	if (image == nullptr)
		ImGui::Text("No Canvas Found");
	else
		ImGui::Text("Canvas Found");
	
}

void ComponentImage::SetImage(uint texture_id, float2 texture_size)
{
	if (image != nullptr)
	{
		image->SetImage(texture_id, SuStoVec2(texture_size.x, texture_size.y));

		//image->SetAnchor(SuStoVec2(0.0f, 0.0f));
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
