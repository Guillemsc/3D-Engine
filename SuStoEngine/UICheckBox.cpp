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
}

void UICheckBox::CleanUp()
{
}

void UICheckBox::OnEvent(UIEvent ev)
{
}

void UICheckBox::SetCheckTrueImage(uint id, SuStoVec2 size)
{
}

void UICheckBox::SetCheckFalseImage(uint id, SuStoVec2 size)
{
}

void UICheckBox::SetState(const bool state)
{
	if (state != checkbox_state)
	{
		checkbox_state = state;
		
		if (checkbox_state) 
			image->SetTexture(check_true);
		else
			image->SetTexture(check_false);
	}
}

const bool UICheckBox::GetState() const
{
	return checkbox_state;
}
