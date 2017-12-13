#include "UICheckBox.h"

UICheckBox::UICheckBox(SuStoUIMain * main, UICanvas* canvas) : UIElement(main, ElementType::CHECKBOX, canvas)
{
	LOG_OUTPUT("Check Box Created");
}

UICheckBox::~UICheckBox()
{
}

void UICheckBox::Awake()
{
}

void UICheckBox::Start()
{
}

void UICheckBox::Update()
{
	if (checkbox_state)
		SetToTrue();
	else
		SetToFalse();
}

void UICheckBox::CleanUp()
{
}

void UICheckBox::OnEvent(UIEvent ev)
{
}

void UICheckBox::SetCheckFalseImage(uint id, SuStoVec2 pos, SuStoVec2 size)
{
	if (check_false != nullptr)
	{
		GetUIMain()->DestroyPrintable(check_false);
		check_false = nullptr;
	}

	check_false = new PrintableElement(id, size, pos, this);
}

void UICheckBox::SetCheckTrueImage(uint id, SuStoVec2 pos, SuStoVec2 size)
{
	if (check_true != nullptr)
	{
		GetUIMain()->DestroyPrintable(check_true);
		check_true = nullptr;
	}

	check_true = new PrintableElement(id, size, pos, this);
}

void UICheckBox::SetToFalse()
{
	if (current_state != check_false)
	{
		GetUIMain()->DestroyPrintable(current_state);
		current_state = check_false;
		GetUIMain()->DrawPrintable(current_state);
	}
}

void UICheckBox::SetToTrue()
{
	if (current_state != check_true)
	{
		GetUIMain()->DestroyPrintable(current_state);
		current_state = check_true;
		GetUIMain()->DrawPrintable(current_state);
	}
}

const bool UICheckBox::GetState() const
{
	return checkbox_state;
}
