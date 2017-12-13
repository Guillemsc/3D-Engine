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
		image->SetTransform(GetOwner()->transform->GetGlobalTransform().Transposed());

		//float2 start = float2::zero;
		//start.x = GetOwner()->transform->GetGlobalTransform()[3][0] - GetOwner()->transform->GetLocalTransform()[3][0];
		//start.y = GetOwner()->transform->GetGlobalTransform()[3][1] - GetOwner()->transform->GetLocalTransform()[3][1];

		//float2 new_local = float2::zero;
		//new_local.x = image->GetCanvas()->GetTransform()[3][0] - start.x;
		//new_local.y = image->GetCanvas()->GetTransform()[3][1] - start.y;

		//float4x4 local = GetOwner()->transform->GetLocalTransform();
		//local[3][0] = new_local.x;
		//local[3][1] = new_local.y;

		//GetOwner()->transform->SetLocalTransform(local);
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
