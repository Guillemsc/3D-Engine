#include "UIButton.h"

UIButton::UIButton(SuStoUIMain * main, UICanvas* canvas) : UIElement(main, ElementType::BUTTON, canvas)
{
	LOG_OUTPUT("Button Created");
}

UIButton::~UIButton()
{
}

void UIButton::Awake()
{
}

void UIButton::Start()
{
}

void UIButton::Update()
{
}

void UIButton::CleanUp()
{
}

void UIButton::OnEvent(UIEvent ev)
{
}
