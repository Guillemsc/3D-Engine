#include "UIPanel.h"

UIPanel::UIPanel(SuStoUIMain * main, UICanvas* canvas) : UIElement(main, ElementType::PANEL, canvas)
{
	LOG_OUTPUT("Panel Created");
}

UIPanel::~UIPanel()
{
}

void UIPanel::Awake()
{
}

void UIPanel::Start()
{
}

void UIPanel::Update()
{
}

void UIPanel::CleanUp()
{
}

void UIPanel::OnEvent(UIEvent ev)
{
	if (ev.mouse_over.element == this || ev.mouse_click.element == this || ev.GetType() == UIEventType::MOUSE_OUT)
	{

	}
}
