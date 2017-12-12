#include "ComponentCanvas.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "SuStoUI.h"
#include "UICanvas.h"

ComponentCanvas::ComponentCanvas(GameObject * owner, std::string unique_id) : Component(ComponentType::UI_CANVAS, owner, unique_id)
{
	LOG_OUTPUT("Component UICanvas Created");
	SetName("UICanvas");

	canvas = App->gameobj->GetUIMain()->CreateCanvas(SuStoVec2(100, 100));
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::Start()
{
}

void ComponentCanvas::Update()
{
	canvas->SetTransform(GetOwner()->transform->GetGlobalTransform().Transposed().ptr());
}

void ComponentCanvas::CleanUp()
{
}

void ComponentCanvas::InspectorDraw(std::vector<Component*> components)
{
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
