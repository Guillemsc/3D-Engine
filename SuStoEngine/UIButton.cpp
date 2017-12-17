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
	SetState(state);
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
	{
		if (ev.mouse_over.element == this)
		{
			SetState(ButtonState::OVER);
		}
	}
		break;
	case UIEventType::MOUSE_OUT:
	{
		if (ev.mouse_out.element == this)
		{
			SetState(ButtonState::IDLE);
		}
	}
		break;
	case UIEventType::MOUSE_CLICK:
	{
		if (ev.mouse_click.element == this)
		{
			SetState(ButtonState::PRESSED);
		}
	}
		break;
	default:
		break;
	}
	
}

void UIButton::SetIdleImage(uint id, SuStoVec2 size)
{
	idle = SuStoTexture(id, size);
}

void UIButton::SetOverImage(uint id, SuStoVec2 size)
{
	over = SuStoTexture(id, size);
}

void UIButton::SetPressedImage(uint id, SuStoVec2 size)
{
	pressed = SuStoTexture(id, size);
}

int UIButton::GetIdleId() const
{
	return idle.id;
}

int UIButton::GetOverId() const
{
	return over.id;
}

int UIButton::GetPressedId() const
{
	return pressed.id;
}

SuStoVec2 UIButton::GetIdleSize() const
{
	return idle.size;
}

SuStoVec2 UIButton::GetOverSize() const
{
	return over.size;
}

SuStoVec2 UIButton::GetPressedSize() const
{
	return pressed.size;
}

void UIButton::SetState(ButtonState state)
{
	this->state = state;

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
		image->SetTexture(idle);
		break;
	}
}

ButtonState UIButton::GetState()
{
	return state;
}

PrintableElement * UIButton::GetImage()
{
	return image;
}
