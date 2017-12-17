#include "UICheckBox.h"
#include "DebugDraw.h"

UICheckBox::UICheckBox(SuStoUIMain * ui_main, UICanvas* canvas) : UIElement(ui_main, ElementType::CHECKBOX, canvas)
{
	LOG_OUTPUT("Check Box Created");

	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(19, 19), this);
	ui_main->DrawPrintable(image);

	SetState(false);
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

void UICheckBox::PreUpdate()
{
}

void UICheckBox::Update()
{
}

void UICheckBox::PostUpdate()
{
	SetState(state);
}

void UICheckBox::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
}

void UICheckBox::OnEvent(UIEvent ev)
{
	switch (ev.GetType())
	{
	case UIEventType::MOUSE_CLICK:
		SetState(!state);
		break;
	default:
		break;
	}
}

void UICheckBox::SetCheckTrueImage(uint id, SuStoVec2 size)
{
	check_true = SuStoTexture(id, size);
}

void UICheckBox::SetCheckFalseImage(uint id, SuStoVec2 size)
{
	check_false = SuStoTexture(id, size);
}

void UICheckBox::SetState(const bool state)
{
	if (this->state != state)
	{
		this->state = state;
		
		if (this->state)
			image->SetTexture(check_true);
		else
			image->SetTexture(check_false);
	}
}

const bool UICheckBox::GetState() const
{
	return state;
}

PrintableElement * UICheckBox::GetImage()
{
	return image;
}
