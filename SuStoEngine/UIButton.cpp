#include "UIButton.h"
#include "DebugDraw.h"

UIButton::UIButton(SuStoUIMain * ui_main, UICanvas* canvas) : UIElement(ui_main, ElementType::BUTTON, canvas)
{
	LOG_OUTPUT("Button Created");

	image = new PrintableElement(0, SuStoVec2(0, 0), SuStoVec2(90, 30), this);
	ui_main->DrawPrintable(image);

	SetState(ButtonState::IDLE);
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
	SetState(ButtonState::IDLE);
}

void UIButton::CleanUp()
{
	GetUIMain()->DestroyPrintable(image);
}

void UIButton::OnEvent(UIEvent ev)
{
	switch (ev.GetType())
	{
	case UIEventType::MOUSE_OVER:
		SetState(ButtonState::OVER);
		break;
	default:
		break;
	}
}

void UIButton::SetIdleImage(uint id, SuStoVec2 size)
{
	idle = SuStoTexture(id, size);
	//SetState(ButtonState::IDLE);
}

void UIButton::SetOverImage(uint id, SuStoVec2 size)
{
	over = SuStoTexture(id, size);
	SetState(ButtonState::OVER);
}

void UIButton::SetPressedImage(uint id, SuStoVec2 size)
{
	pressed = SuStoTexture(id, size);
}

void UIButton::SetState(ButtonState state)
{
	switch (state)
	{
	case IDLE:
		image->SetTexture(idle);
		break;
	case OVER:
		image->SetTexture(over);
		break;
	case PRESSED:
		image->SetTexture(pressed);
		break;
	default:
		break;
	}
}
