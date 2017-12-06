#include "UICanvas.h"

UICanvas::UICanvas(SuStoUIMain * main) : UIElement(main, ElementType::CANVAS)
{
	LOG_OUTPUT("Canvas Created");
}

UICanvas::~UICanvas()
{
}

void UICanvas::Awake()
{
}

void UICanvas::Start()
{
}

void UICanvas::Update()
{
}

void UICanvas::CleanUp()
{
}

void UICanvas::OnEvent(UIEvent ev)
{
}

void UICanvas::SetWidthAndHeight(float width, float height)
{

}
