#include "ComponentCanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "SuStoUI.h"
#include "UICanvas.h"
#include "UIImage.h"
#include "imgui.h"
#include "DebugDraw.h"
#include "Color.h"

ComponentCanvas::ComponentCanvas(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_CANVAS, owner, unique_id)
{
	LOG_OUTPUT("Component UICanvas Created");
	SetName("UICanvas");

	owner->SetName("Canvas");

	canvas = App->gameobj->GetUIMain()->CreateCanvas();
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::Start()
{
}

void ComponentCanvas::Update()
{
	canvas->SetTransform(GetOwner()->transform->GetGlobalTransform());

	DebugDraw(canvas->GetPlane()->GetBbox(), Red, true, 4.0f, canvas->GetPlane()->GetTransform());
}

void ComponentCanvas::CleanUp()
{
	App->gameobj->GetUIMain()->DeleteCanvas(canvas);
}

void ComponentCanvas::InspectorDraw(std::vector<Component*> components)
{
	const char* items[] = { "World Space", "Camera Space" };
	static int item2 = 0;
	ImGui::Combo("Render Space", &item2, items, IM_ARRAYSIZE(items));  
	{
		switch (item2)
		{
		case 0:
			canvas->SetRenderMode(UICanvasRenderMode::WORLD_SPACE);
			break;
		case 1:
			canvas->SetRenderMode(UICanvasRenderMode::CAMERA_SPACE);
			break;
		}
	}

	if (item2 == 1)
	{
		bool show = canvas->GetShowOnCamera();
		if(ImGui::Checkbox("Show on camera", &show))
		{
			canvas->SetShowOnCamera(show);
		}
	}
	else
		canvas->SetShowOnCamera(false);

	ImGui::Separator();

	if (ImGui::Button("Add Image"))
	{
		GameObject* go = App->gameobj->Create();
		go->AddComponent(ComponentType::UI_IMAGE);
		GetOwner()->AddChild(go);
	}
}

UICanvas * ComponentCanvas::GetCanvas()
{
	return canvas;
}

void ComponentCanvas::OnEnable()
{
}

void ComponentCanvas::OnDisable()
{
}

void ComponentCanvas::OnLoadSerialize(JSON_Doc config)
{
}

void ComponentCanvas::OnSaveSerialize(JSON_Doc config)
{
}
