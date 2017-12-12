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

void UIButton::PreUpdate()
{
}

void UIButton::Update()
{
}

void UIButton::PostUpdate()
{
}

void UIButton::CleanUp()
{
}

void UIButton::OnEvent(UIEvent ev)
{
}

void UIButton::SetStandardImage(uint id, SuStoVec2 pos, SuStoVec2 size)
{
	if (standard != nullptr)
	{
		GetUIMain()->DestroyPrintable(standard);
		standard = nullptr;
	}

	standard = new PrintableElement(id, size, pos, this);
}

void UIButton::SetHighlightImage(uint id, SuStoVec2 pos, SuStoVec2 size)
{
	if (highlight != nullptr)
	{
		GetUIMain()->DestroyPrintable(highlight);
		highlight = nullptr;
	}

	highlight = new PrintableElement(id, size, pos, this);
}

void UIButton::SetClickImage(uint id, SuStoVec2 pos, SuStoVec2 size)
{
	if (click != nullptr)
	{
		GetUIMain()->DestroyPrintable(click);
		click = nullptr;
	}

	click = new PrintableElement(id, size, pos, this);
}

void UIButton::SetToStandard()
{
	if (current_state != standard)
	{
		GetUIMain()->DestroyPrintable(current_state);
		current_state = standard;
		state = ButtonState::STANDARD;
		GetUIMain()->DrawPrintable(current_state);
	}
}

void UIButton::SetToHighlight()
{
	if (current_state != highlight) 
	{
		GetUIMain()->DestroyPrintable(current_state);
		current_state = highlight;
		state = ButtonState::HIGHLIGHT;
		GetUIMain()->DrawPrintable(current_state);
	}
}

void UIButton::SetToClick()
{
	if (current_state != click)
	{
		GetUIMain()->DestroyPrintable(current_state);
		current_state = click;
		state = ButtonState::CLICK;
		GetUIMain()->DrawPrintable(current_state);
	}
}
